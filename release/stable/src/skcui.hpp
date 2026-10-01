#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <optional>
#include <utility>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

namespace skcui {


    inline constexpr char KEY_UP    = '\x01';
    inline constexpr char KEY_DOWN  = '\x02';
    inline constexpr char KEY_ENTER = '\n';
    inline constexpr char KEY_ESC   = '\x1b';

    inline char getKey()
    {
#ifdef _WIN32
        int key = _getch();

        if (key == '\r')
            return KEY_ENTER;

        if (key == 0 || key == 224)
        {
            key = _getch();

            if (key == 72) return KEY_UP;
            if (key == 80) return KEY_DOWN;
        }

        if (key == 27)
            return KEY_ESC;

        return key;

#elif defined(__linux__) || defined(__APPLE__)
        termios oldt, newt;

        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;

        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        char key = getchar();

        if (key == '\n')
            key = KEY_ENTER;

        if (key == KEY_ESC) {
            char sequence[2]{};

            if (read(STDIN_FILENO, sequence, 2) == 2 &&
                sequence[0] == '[') {
                if (sequence[1] == 'A') key = KEY_UP;
                if (sequence[1] == 'B') key = KEY_DOWN;
            }
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

        return key;

#else
        return '\0';
#endif
    }

    inline void clearScreen() {
#ifdef _WIN32
        system("cls");
#elif defined(__linux__) || defined(__APPLE__)
        std::cout << "\033[2J\033[1;1H" << std::flush;
#endif
    }

    inline void menu(
        int& selected,
        const std::vector<std::string>& options,
        const std::optional<std::string_view> title = std::nullopt,
        const std::optional<std::string_view> desc = std::nullopt) {

        if (options.empty())
            return;

        selected = 0;
        bool running = true;

        while (running) {

            clearScreen();

            if (title) {
                std::cout << *title << std::endl << std::endl;

                if (desc) {
                    std::cout << *desc << std::endl << std::endl;
                }
            }

            for (int x = 0; x < options.size(); x++) {
                if (selected == x) {
                    std::cout << "> ";
                }

                std::cout << options[x] << std::endl;
            }

            const char key = getKey();

            if (key == KEY_UP && selected > 0) {
                selected--;
            }
            else if (key == KEY_DOWN &&
                     selected < static_cast<int>(options.size()) - 1) {
                selected++;
            }
            else if (key == KEY_ENTER || key == KEY_ESC) {
                running = false;
            }
        }
    }

    inline void checkbox(
        int& selected,
        std::vector<std::pair<std::string, bool>>& options,
        const std::optional<std::string_view> title = std::nullopt) {

        if (options.empty())
            return;

        selected = 0;
        bool running = true;

        while (running) {

            clearScreen();

            if (title) {
                std::cout << *title << std::endl << std::endl;
            }

            for (int x = 0; x < options.size(); x++) {
                if (selected == x) {
                    std::cout << "> ";
                }

                if (options[x].second) {
                    std::cout << "[X] ";
                }
                else {
                    std::cout << "[ ] ";
                }

                std::cout << options[x].first << std::endl;
            }

            const char key = getKey();

            if (key == KEY_UP && selected > 0) {
                selected--;
            }
            else if (key == KEY_DOWN &&
                     selected < static_cast<int>(options.size()) - 1) {
                selected++;
            }
            else if (key == KEY_ENTER) {
                options[selected].second = !options[selected].second;
            }
            else if (key == KEY_ESC) {
                running = false;
            }
        }
    }
}
