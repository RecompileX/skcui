# SKCUI

SKCUI is a small, header-only C++ library for interactive console menus. Version 0.1.0 provides arrow-key navigation and Enter-to-select behavior on Windows, Linux, and macOS terminal APIs.

## Requirements

- A C++17-compatible compiler
- A terminal that supports ANSI clear-screen escape sequences
- Windows, Linux, or macOS for interactive keyboard input

## Quick start

Copy `release/src/skcui.hpp` into your project or add `release/src` to your compiler's include path:

```cpp
#include "skcui.hpp"

#include <iostream>
#include <string>
#include <vector>

int main() {
    const std::vector<std::string> options{"Start", "Exit"};
    int selected = 0;

    skcui::menu(selected, options, "Main menu", "Choose an option.");

    if (selected >= 0) {
        std::cout << "Selected: " << options[static_cast<std::size_t>(selected)] << '\n';
    }
}
```

With an x64 Visual Studio Developer Command Prompt:

```bat
cl /nologo /std:c++17 /EHsc /W4 /I release\src release\example\example.cpp /Fe:skcui-example.exe
```

Run the executable, navigate with Up and Down, and press Enter to select.

## Repository layout

- `dev/` — Visual Studio development solution and project
- `release/` — self-contained v0.1.0 distribution
- `docs/` — API documentation, release notes, and project design records

Open `dev/skcui.slnx` for Visual Studio development. The distributable is documented independently in [`release/README.md`](release/README.md).

## Documentation

- [API reference](docs/api.md)
- [v0.1.0 release notes](docs/releases/v0.1.0.md)
- [Changelog](CHANGELOG.md)

## Current limitations

- Interactive input is synchronous.
- ANSI clear-screen sequences require a compatible terminal.
- Linux and macOS code paths are included but were not executed as part of this Windows release build.

## License

SKCUI is available under the [MIT License](LICENSE).
