#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <limits>
#include <optional>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#define KEY_UP    '\x01'
#define KEY_DOWN  '\x02'
#define KEY_ENTER '\n'

namespace skcui {
    class display
    {
    public:
        void render(const menu& men)
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
        void render(const checkbox& cb)
        {
            // TODO: Finish this and replace goto loop with the run ui loop.
            cb.selected = 0;
            bool running = true;

            loop:
                clearScreen();

            for (int x = 0; x < cb.checkboxName.size(); x++) {
                if (men.selected == x) {
                    std::cout << "> ";
                }

                std::cout << "[ ]" << std::endl;
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
        }
    };
    class component {
    public:
        struct menu {
            int& selected;
            const std::vector<std::string>& options;
            const std::optional<std::string> title = std::nullopt;
            const std::optional<std::string> desc = std::nullopt;
        };

        struct checkbox {
            std::vector<bool> checked;
            std::vector<std::string> checkboxName;
        };
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

    char key = getchar();

    if (key == '\n')
        key = KEY_ENTER;

    if (key == '\033')
    {
        getchar(); // '['
        key = getchar();

        if (key == 'A') key = KEY_UP;
        if (key == 'B') key = KEY_DOWN;
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

template<typename T, typename... K>
inline void runUi(T t, K... k)
{
    if ()
}
}
