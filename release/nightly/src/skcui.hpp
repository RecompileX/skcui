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

inline constexpr char KEY_UP = '\x01';
inline constexpr char KEY_DOWN = '\x02';
inline constexpr char KEY_ENTER = '\n';
inline constexpr char KEY_ESC = '\x1b';

namespace skcui {

    inline char getKey();
    inline void clearScreen();

    namespace component {
        struct menu {
            int selected;
            const std::vector<std::string>& options;
            const std::optional<std::string_view> title = std::nullopt;
            const std::optional<std::string_view> desc = std::nullopt;
        };

        struct checkbox {
            std::vector<std::pair<std::string, bool>> checkboxName;
            int selected;
        };
    }
    template<typename T, typename ... U>
    concept either = (std::same_as<T, U> || ...);

    template<typename T>
    concept isUi = either<std::remove_cvref_t<T>, component::menu, component::checkbox>;

    namespace display{
        inline void render(component::menu& men)
        {
            if(men.options.size() == 0)
                return;
            men.selected = 0;
            bool running = true;

        while (running)
        {
        
            clearScreen();

            if (men.title) {
                std::cout << *men.title << std::endl << std::endl;

                if (men.desc) {
                    std::cout << *men.desc << std::endl << std::endl;
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
        }
    }
        inline void render(component::checkbox& cb)
        {
            if(cb.checkboxName.size() == 0)
                return;
            
            cb.selected = 0;
            bool running = true;

            while(running){
                clearScreen();

                for (int x = 0; x < cb.checkboxName.size(); x++) {
                    if (cb.selected == x) {
                        std::cout << "> ";
                    }
                    if (cb.checkboxName[x].second) {
                        std::cout << "[X] " << cb.checkboxName[x].first << std::endl;
                    }
                    else {
                        std::cout << "[ ] " << cb.checkboxName[x].first << std::endl;
                    }
                }
                const char key = getKey();
                if (key == KEY_UP && cb.selected > 0) {
                    cb.selected--;
                }
                else if (key == KEY_DOWN && cb.selected < cb.checkboxName.size() - 1) {
                    cb.selected++;
                }
                else if (key == KEY_ENTER) {
                    cb.checkboxName[cb.selected].second = !cb.checkboxName[cb.selected].second;
                }
                else if (key == KEY_ESC) {
                    running = false;
                }
            }
        }
    };
};
namespace skcui {

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
    template<isUi... T>
    inline void runUi(T&... t)
    {
        (display::render(t), ...);
    };
}
