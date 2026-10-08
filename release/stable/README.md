# SKCUI Stable

**SKCUI — Simple Keyboard Console UI** is a header-only C++ console UI library. This package contains the current component API with menus, checkboxes, text input, separators, and progress bars.

## Install

Copy `src/skcui.hpp` into your project or add `src` to the compiler include path. The header requires C++20 or newer.

```cpp
#include "skcui.hpp"

int main() {
    skcui::component::Menu menu{{"Start", "Exit"}};
    menu.add(skcui::component::Text{"Choose an option."});
    skcui::display::render(menu);
}
```

## Build the example

From this directory in an x64 Visual Studio Developer Command Prompt:

```bat
cl /nologo /std:c++20 /EHsc /W4 /I src examples\full-example.cpp /Fe:skcui-example.exe
```

The bundled CMake project requires CMake 3.20+ and configures the example for C++23:

```sh
cmake -S examples -B build
cmake --build build --config Release
```

The example demonstrates a menu with text, input, a separator, and a progress bar, followed by checkboxes and a standalone progress bar.

## Controls

- Up/Down moves through menu options, checkbox options, and input add-ons.
- Enter chooses a menu option or toggles a checkbox.
- Escape closes the current UI.
- Typing and Backspace edit the focused input.

The default console clearing uses ANSI sequences. Define `SKCUILEGACYCONSOLE` before including the header to use the platform's clear command.

See the [API reference](../../docs/api.md) for component fields, add-ons, and retrieving input. For the C++17 function API, use [stable 1.x](../stable%201.x/README.md).

## License

[MIT License](LICENSE).
