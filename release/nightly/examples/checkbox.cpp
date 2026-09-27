#include "skcui.hpp"

#include <iostream>
#include <utility>
#include <vector>

int main() {
    skcui::component::checkbox choices{
        {},
        {{"Music", true}, {"Sound effects", false}, {"Fullscreen", false}},
        0
    };

    skcui::runUi(choices);

    for (const auto& [name, checked] : choices.checkboxName) {
        std::cout << (checked ? "[X] " : "[ ] ") << name << '\n';
    }
}
