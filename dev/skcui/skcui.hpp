#pragma once

#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>


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
    inline void clearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    };

    inline std::string progressBar(float progress, float min,float max, std::optional<char> symbol, std::optional<int> width) {
        std::string result = "{";
        if (!symbol)
            symbol = '*';
        if (!width)
            width = 10;

        float x = (progress - min) / (max - min);
        int amount = x * *width;
        for (std::size_t y = 0; y < amount; y++) {
            result.push_back(*symbol);
        }
        result.push_back('}');
        return result;
    }

    namespace component {

        struct Text {
            std::string text = " ";
        };

        struct Input {
            std::string value;
            std::string prompt = "Prompt not initialized! Set to “- 1” to not render a prompt.";
        };

        struct Separator {
            char symbol = '-';
            std::size_t width = 20;
        };

        struct BlankSeparator {
            std::size_t separatorAmount = 1;
        };

        struct ProgressBar {
            char symbol = '*';
            float progress = 0, min = 0, max = 0;
            std::size_t width = 20;
        };
        
        struct Component {
            using Child = std::variant<Text, Input, Separator, BlankSeparator, ProgressBar>;
            std::vector<Child> addOn;
            
            inline void add(const Child& child) {
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
            char symbol = '>';

        };
        
        struct Menu : Component {
            const std::vector<std::string>& options;
            int selected = 0;

        };

        struct Checkbox : Component {
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
            std::cout << std::string(sep.width, sep.symbol) << std::endl;
        }
        inline void subRender(component::BlankSeparator& blankSep) {
            std::cout << std::string(blankSep.separatorAmount, '\n');
        }
        inline void subRender(component::Input& input, component::Menu& menu, std::size_t x, char key) {
            if (input.prompt != "- 1") {
                std::cout << input.prompt;
            }
            if (x == menu.selected) {
                if (key == KEY_ENTER) {
                    return;
                }
                if (key == '\b') {
                    if (!input.value.empty()) {
                        input.value.pop_back();
                    }
                }
                else if (key != KEY_ESC) {
                    input.value += key;
                }
            }
                std::cout << menu.symbol << ' ';
                std::cout << input.value << std::endl;
        }
        inline void subRender(component::ProgressBar& pb) {
            float x = (pb.progress - pb.min) / (pb.max - pb.min);
            int amount = x * pb.width;
            std::cout << '{';
            for (std::size_t y = 0; y < amount; y++) {
                std::cout << pb.symbol;
            }
            std::cout << '}';
        }
        
        inline void render(component::Menu& menu)
        {
            if (menu.options.empty()) {
                return;
            }
            bool running = true;
            std::size_t inputAddonAmount = 0;

            for (std::size_t x = 0; x < menu.addOn.size(); x++) {
                std::visit([&inputAddonAmount](auto& child) {
                    using Child = std::remove_cvref_t<decltype(child)>;
                    if constexpr (std::is_same_v<Child, component::Input>) {
                        inputAddonAmount++;
                    }
                }, menu.addOn[x]);
            }

            while (running)
            {

                clearScreen();

                for (std::size_t x = 0; x < menu.options.size(); x++) {
                    if (menu.selected == x) {
                           std::cout << menu.symbol << ' ';
                    }
                    std::cout << menu.options[x] << std::endl;
                }
                const char key = getKey();
                if (key == KEY_UP && menu.selected > 0) {
                    menu.selected--;
                }
                else if (key == KEY_DOWN && menu.selected < menu.options.size() + inputAddonAmount - 1) {
                    menu.selected++;
                }
                else if (key == KEY_ENTER && menu.selected < menu.options.size()) {
                    running = false;
                }
                std::size_t y = 0;
                for (std::size_t x = 0; x < menu.addOn.size(); x++) {
                    std::visit([&menu, &y](auto& child) {
                        using Child = std::remove_cvref_t<decltype(child)>;
                        if constexpr (std::is_same_v<Child, component::Input>) {
                            subRender(child, menu, y, key);
                            y++;
                        }
                        else {
                            subRender(child);
                        }

                    },
                        menu.addOn[x]
                    );
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
                        std::cout << cb.symbol << ' ';
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
                for (std::size_t x = 0; x < cb.addOn.size(); x++) {
                    std::visit([&cb](auto& child) {
                        using Child = std::remove_cvref_t<decltype(child)>;
                        if constexpr (std::is_same_v<Child, component::Input>) {
                            subRender(child, cb);
                        }
                        else {
                            subRender(child);
                        }

                    }, cb.addOn[x]);
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

        if (key == 8) {
            return '\b';
        }

        if (key == 27) {
            return KEY_ESC;
        }

        if (key == 0 || key == 224)
        {
            key = _getch();

            if (key == 72) return KEY_UP;
            if (key == 80) return KEY_DOWN;
        }

        return static_cast<char>(key);

#elif defined(__linux__) || defined(__APPLE__)
        termios oldt, newt;

        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;

        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        unsigned char key = 0;
        read(STDIN_FILENO, &key, 1);

        if (key == '\n' || key == '\r') {
            key = KEY_ENTER;
        }

        if (key == 127 || key == 8) {
            key = '\b';
        }

        if (key == KEY_ESC) {
            fd_set input;
            FD_ZERO(&input);
            FD_SET(STDIN_FILENO, &input);

            timeval timeout{};
            timeout.tv_usec = 30000;

            if (select(STDIN_FILENO + 1, &input, nullptr, nullptr, &timeout) > 0)
            {
                unsigned char sequence[2]{};

                if (read(STDIN_FILENO, sequence, 2) == 2 &&
                    sequence[0] == '[')
                {
                    if (sequence[1] == 'A') key = KEY_UP;
                    if (sequence[1] == 'B') key = KEY_DOWN;
                }
            }
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

        return static_cast<char>(key);

#else
        return '\0';
#endif
    }
}