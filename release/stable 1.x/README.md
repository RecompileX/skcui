# SKCUI Stable 1.x

**SKCUI — Simple Keyboard Console UI** is a header-only C++ console UI library. This package preserves the C++17 function API with keyboard-driven menus and checkboxes.

## Install

Copy `src/skcui.hpp` into your project or add `src` to the compiler include path. This package requires C++17 or newer.

```cpp
#include "skcui.hpp"

int main() {
    const std::vector<std::string> options{"Start", "Exit"};
    int selected = 0;
    skcui::menu(selected, options, "Main menu", "Choose an option.");
}
```

## Build the example

From this directory in an x64 Visual Studio Developer Command Prompt:

```bat
cl /nologo /std:c++17 /EHsc /W4 /I src examples\full-example.cpp /Fe:skcui-1x-example.exe
```

Or use CMake 3.20+:

```sh
cmake -S examples -B build
cmake --build build --config Release
```

The example opens a Launch/Settings/Exit menu and lets you change checkbox settings before launching.

## Controls

- Menu: Up/Down moves; Enter or Escape returns with the highlighted index.
- Checkbox: Up/Down moves; Enter toggles; Escape returns.

Selection is reset to the first option whenever a non-empty menu or checkbox list is opened. Checkbox values are updated in place.

See the [API reference](docs/api.md). For composable inputs, separators, and progress bars, use the [current stable package](../stable/README.md).

## License

[MIT License](LICENSE).
