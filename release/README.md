# SKCUI v0.1.0

**SKCUI** stands for **Simple Keyboard Cursor User Interface**.

This directory is the self-contained SKCUI v0.1.0 distribution.

## Add SKCUI to a project

Copy `src/skcui.hpp` into your project, or add this directory's `src` folder to your compiler include path. Then include:

```cpp
#include "skcui.hpp"
```

SKCUI is header-only and requires C++17 or newer.

## Use a menu

```cpp
#include "skcui.hpp"

#include <string>
#include <vector>

int main() {
    const std::vector<std::string> options{"Start", "Settings", "Exit"};
    int selected = 0;

    skcui::menu(selected, options, "SKCUI", "Use the arrow keys and Enter.");
}
```

Up and Down move the selection; Enter accepts it. After return, `selected` is the zero-based selected index. Pass at least one option.

## Compile the included example

From this `release` directory in an x64 Visual Studio Developer Command Prompt:

```bat
cl /nologo /std:c++17 /EHsc /W4 /I src example\example.cpp /Fe:example\skcui-example.exe
```

Run `example\skcui-example.exe` in a terminal.

## Platform support

Interactive input is implemented for Windows, Linux, and macOS. The v0.1.0 build was verified with MSVC on Windows; Linux and macOS code paths were not executed on the release host.

## License

SKCUI is distributed under the MIT License. Copyright (c) 2026 Hamza Kamal. See `LICENSE` in this release directory.
