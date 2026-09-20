# SKCUI API Reference

SKCUI v0.1.0 is a header-only C++17 library. Include `skcui.hpp`; no separate library link step is required.

## Key values

`KEY_UP`, `KEY_DOWN`, and `KEY_ENTER` identify the input values used by the menu.

## `getKey`

```cpp
char getKey();
```

Waits for one keyboard event and normalizes Up, Down, and Enter. Windows uses `_getch`; Linux and macOS use terminal input APIs.

## `skcui::clearScreen`

```cpp
void clearScreen();
```

Writes an ANSI clear-screen and cursor-home sequence to standard output.

## `skcui::menu`

```cpp
void menu(
    int& selected,
    const std::vector<std::string>& options,
    std::optional<std::string_view> title = std::nullopt,
    std::optional<std::string_view> desc = std::nullopt);
```

Displays an interactive, non-empty option list. Up and Down move the selection; Enter returns with `selected` set to the chosen zero-based index. One keyboard event is read per menu loop.
