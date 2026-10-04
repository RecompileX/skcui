#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <optional>
#include <concepts>
#include <type_traits>
#include <cstddef>
#include <utility>
#include <variant>

#ifdef _WIN32
#include <conio.h>
#else
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#endif

namespace skcui {

    inline constexpr char KEY_UP = '\x01';
    inline constexpr char KEY_DOWN = '\x02';
    inline constexpr char KEY_ENTER = '\n';
    inline constexpr char KEY_ESC = '\x1b';

    inline char getKey();
    inline void clearScreen();
    
    inline std::string getln(){
        std::string buffer;
        std::getline(std::cin, buffer);
        return buffer;
    }

    inline void getln(std::string& buffer) {
        std::getline(std::cin, buffer);
    }

    namespace component {

        struct Text {
            std::string text;
        };

        struct Input {
            std::string& value;
            std::string prompt = " - 1";
        };

        struct Separator {
            char symbol = '-';
            std::size_t width = 20;
        };

        struct BlankSeparator {
            int sepratorAmount = 1;
        };
        
        struct Container {
            using Child = std::variant<Text, Input, Separator>;
            std::vector<Child> addOn;
            
            inline void add(Child& child) {
                addOn.push_back(child);
            }
            inline void remove(int index) {
                if (index >= 0 && index < addOn.size()) {
                    addOn.erase(addOn.begin() + index);
                }
            }
            inline void remove() {
                if (!addOn.empty()) {
                    addOn.pop_back();
                }
            }

        };
        
        struct Menu : Container {
            const std::vector<std::string>& options;
            
            int selected = 0;

        };

        struct Checkbox : Container {
            std::vector<std::pair<std::string, bool>> checkboxName;
            int selected = 0;
        };
    }
    template<typename T, typename ... U>
    concept either = (std::same_as<T, U> || ...);

    template<typename T>
    concept isUi = either<std::remove_cvref_t<T>, component::Menu, component::Checkbox>;
        
    namespace display {
        inline void subRender(component::Text& text) {
            std::cout << text.text << std::endl;
        }
        inline void subRender(component::Separator& sep) {
            std::cout << std::endl << sep.symbol << std::endl;
        }
        inline void render(component::Menu& menu)
        {
            if (menu.options.empty()) {
                return;
            }
            bool running = true;

            while (running)
            {

                clearScreen();

                for (std::size_t x = 0; x < menu.options.size() - 1; x++) {
                    if (menu.selected == x) {
                           std::cout << "> ";
                    }
                    std::cout << menu.options[x] << std::endl;
                }
                const char key = getKey();
                if (key == KEY_UP && menu.selected > 0) {
                    menu.selected--;
                }
                else if (key == KEY_DOWN && menu.selected < menu.options.size() - 1) {
                    menu.selected++;
                }
                else if (key == KEY_ENTER) {
                    running = false;
                }
            }
        }
        inline void render(component::Checkbox& cb)
        {
            if (cb.checkboxName.empty())
                return;
            bool running = true;

            while (running) {
                clearScreen();

                for (std::size_t x = 0; x < cb.checkboxName.size(); x++) {
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
                for (auto it : addOn) {
                    subRender(it);
                }
            }
        }
    }
    inline char getKey()
    {
#ifdef _WIN32
        int key = _getch();

        if (key == '\r') {
            return KEY_ENTER;
        }

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

        if (key == '\n') {
            key = KEY_ENTER;
        }

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
    }
}