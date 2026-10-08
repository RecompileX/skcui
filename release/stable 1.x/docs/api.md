# SKCUI Stable 1.x API Reference

**SKCUI** stands for **Simple Keyboard Console UI**. This package preserves the C++17 function API in `src/skcui.hpp`. Include `skcui.hpp`; no separate library link step is required.

For the current component API, see the [main API reference](../../../docs/api.md).

## Keyboard input

```cpp
char skcui::getKey();
```

Waits for a keyboard event and normalizes Up, Down, Enter, and Escape to `KEY_UP`, `KEY_DOWN`, `KEY_ENTER`, and `KEY_ESC`. Windows uses `_getch`; Linux/macOS use terminal input APIs.

## Clearing the console

```cpp
void skcui::clearScreen();
```

Uses `system("cls")` on Windows and ANSI clear-screen/cursor-home sequences on Linux/macOS.

## Menus

```cpp
void skcui::menu(
    int& selected,
    const std::vector<std::string>& options,
    std::optional<std::string_view> title = std::nullopt,
    std::optional<std::string_view> desc = std::nullopt);
```

For a non-empty list, resets `selected` to 0 and displays the options. Up/Down moves the selection; Enter or Escape returns with the current zero-based index. An empty list returns immediately without changing `selected`.

The optional description is displayed only when a title is provided. The function does not report whether Enter or Escape ended the menu.

```cpp
const std::vector<std::string> options{"Start", "Exit"};
int selected = 0;
skcui::menu(selected, options, "Main menu", "Choose an option.");
```

## Checkboxes

```cpp
void skcui::checkbox(
    int& selected,
    std::vector<std::pair<std::string, bool>>& options,
    std::optional<std::string_view> title = std::nullopt);
```

Each pair stores a label and checked state. For a non-empty list, resets `selected` to 0; Up/Down moves; Enter toggles the highlighted bool in place; Escape returns. An empty list returns immediately.

```cpp
std::vector<std::pair<std::string, bool>> settings{
    {"Sound", true},
    {"Animations", false}
};
int selected = 0;
skcui::checkbox(selected, settings, "Settings");
```

Interactive input is synchronous. Include the standard headers used by your own code, such as `<string>`, `<vector>`, and `<utility>`.
