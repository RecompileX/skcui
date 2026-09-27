#include "skcui.hpp"

#include <iostream>
#include <string>
#include <vector>

int main() {
    const std::vector<std::string> options{"Start", "Settings", "Exit"};
    skcui::component::menu menu{0, options, "SKCUI Nightly", "Use Up/Down and Enter."};

    skcui::runUi(menu);
    std::cout << "Selected: " << options.at(static_cast<std::size_t>(menu.selected)) << '\n';
}
