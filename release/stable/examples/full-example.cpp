
#include <iostream>
#include <thread>
#include <chrono>

#include "../src/skcui.hpp"

int main()
{
    using namespace skcui;
    using namespace skcui::component;

    Menu menu{{
        "Launch",
        "Settings",
        "Exit"
    }};

    menu.add(Text{"SKCUI - Simple Keyboard Console UI"});
    menu.add(Separator{'=', 32});
    menu.add(Input{"", "Name: "});
    menu.add(ProgressBar{'#', 70, 0, 100, 20});

    display::render(menu);

    Checkbox settings{{
        {"Animations", true},
        {"Sound", false},
        {"Dark Mode", true}
    }};

    settings.add(Separator{'-', 32});
    settings.add(Text{"ENTER = toggle | ESC = continue"});

    display::render(settings);

    clearScreen();

    std::cout << "Launching SKCUI...\n\n";

    for (int progress = 0; progress <= 100; ++progress)
    {
        std::cout
            << '\r'
            << progressBar(progress, 0, 100, '#', 25)
            << " "
            << progress
            << "%"
            << std::flush;

        std::this_thread::sleep_for(
            std::chrono::milliseconds(20)
        );
    }

    std::cout << "\n\nReady!\n";

    return 0;
}
