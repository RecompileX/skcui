#include "skcui.hpp"
#include <vector>
#include <string>

int main() {
    std::vector<std::string> options = {
        "Start",
        "Settings",
        "About",
        "Exit"
    };

    int selected = 0;

    skcui::menu(
        selected,
        options,
        "SKCUI Example",
        "Use the arrow keys to navigate and Enter to select."
    );

    return 0;
}
