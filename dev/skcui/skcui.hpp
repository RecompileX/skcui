#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <optional>
#include <concepts>
#include <type_traits>

#ifdef _WIN32
#include <conio.h>
#else
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#endif

#define KEY_UP    '\x01'
#define KEY_DOWN  '\x02'
#define KEY_ENTER '\n'
#define KEY_ESC '\x1b'

namespace skcui {
    namespace component {
        struct menu {
            int& selected;
            const std::vector<std::string>& options;
            const std::optional<std::string> title = std::nullopt;
            const std::optional<std::string> desc = std::nullopt;
        };

        struct checkbox {
            std::vector<bool> checked;
            std::vector<std::pair<std::string, bool>> checkboxName;
        };
    }
    template<typename T, typename ... U>
    concept either = (std::same_as<T, U> || ...);

    template<typename T>
    concept isUi = either<std::remove_cvref_t<T>, component::menu, component::checkbox>;
    
    template<isUi... T>
    inline void runUi(T... t)
    {
        display::render(t);
    };

    namespace display{
        void render(component::menu& men)
        {
            men.selected = 0;
            bool running = true;

        loop:
            clearScreen();

            if (men.title) {
                std::cout << men.title << std::endl << std::endl;

                if (men.desc) {
                    std::cout << men.desc << std::endl << std::endl;
                }
            }
            for (int x = 0; x < men.options.size(); x++) {
                if (men.selected == x) {
                    std::cout << "> ";
                }

                std::cout << men.options[x] << std::endl;
            }
            const char key = getKey();
            if (key == KEY_UP && men.selected > 0) {
                men.selected--;
            }
            else if (key == KEY_DOWN && men.selected < men.options.size() - 1) {
                men.selected++;
            }
            else if (key == KEY_ENTER) {
                running = false;
            }
            if (running == true) {
                goto loop;
            }
        }
        void render(component::checkbox& cb)
        {
            // TODO: Finish this and replace goto loop with the run ui loop.
            cb.checkboxName;
            bool running = true;

        loop:
            clearScreen();

            for (int x = 0; x < cb.checkboxName.size(); x++) {
                if (cb.selected == x) {
                    std::cout << "> ";
                }

                std::cout << "[ ]" << std::endl;
                std::cout << "[X]" << std::endl;
            }
            const char key = getKey();
            if (key == KEY_UP && cb.selected > 0) {
                cb.selected--;
            }
            else if (key == KEY_DOWN && cb.selected < cb.options.size() - 1) {
                cb.selected++;
            }
            else if (key == KEY_ENTER) {
                running = false;
            }
            if (running == true) {
                goto loop;
            }
        }
    };
};

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

    return key;

#elif defined(__linux__) || defined(__APPLE__)
    termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    unsigned char key = 0;
    read(STDIN_FILENO, &key, 1);

    if (key == '\n')
        key = KEY_ENTER;

    if (key == KEY_ESC) {
        fd_set input;
        FD_ZERO(&input);
        FD_SET(STDIN_FILENO, &input);

        timeval timeout{};
        timeout.tv_usec = 30000;

        if (select(STDIN_FILENO + 1, &input, nullptr, nullptr, &timeout) > 0) {
            unsigned char sequence[2]{};

            if (read(STDIN_FILENO, sequence, 2) == 2 && sequence[0] == '[') {
                if (sequence[1] == 'A') key = KEY_UP;
                if (sequence[1] == 'B') key = KEY_DOWN;
            }
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return key;

#else
    return '\0';
#endif
}

inline void clearScreen()
{
#ifdef _WIN32
    system("cls");
#elif defined(__linux__) || defined(__APPLE__)
    system("clear");
#endif
}