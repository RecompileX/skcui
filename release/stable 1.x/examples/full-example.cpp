#include <iostream>
#include <string>
#include <vector>
#include <utility>

#include "../src/skcui.hpp"

int main()
{
    using namespace skcui;

    const std::vector<std::string> options{
        "Launch",
        "Settings",
        "Exit"
    };

    std::vector<std::pair<std::string, bool>> settings{
        {"Animations", true},
        {"Sound", false},
        {"Dark Mode", true}
    };

    int selected = 0;
    int settingSelected = 0;

    while (true)
    {
        menu(
            selected,
            options,
            "SKCUI - Simple Keyboard Console UI",
            "Use UP/DOWN to navigate and ENTER to select."
        );
        
        if (selected == 0)
        {
            clearScreen();

            std::cout << "Launching SKCUI...\n\n";
            std::cout << "Current settings:\n\n";

            for (const auto& setting : settings)
            {
                std::cout << setting.first << ": ";

                if (setting.second)
                {
                    std::cout << "Enabled\n";
                }
                else
                {
                    std::cout << "Disabled\n";
                }
            }

            std::cout << "\nReady!\n";
            break;
        }

        else if (selected == 1)
        {
            checkbox(
                settingSelected,
                settings,
                "Settings - ENTER to toggle, ESC to return"
            );
        }

        else if (selected == 2)
        {
            clearScreen();
            std::cout << "Goodbye!\n";
            break;
        }
    }

    return 0;
}