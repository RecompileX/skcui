# SKCUI

**SKCUI** stands for **Simple Keyboard Cursor User Interface**.

SKCUI is a small, header-only C++ library for interactive console menus. It provides keyboard-driven menus on Windows, Linux, and macOS without requiring a separate library build.

## Requirements

- A C++17-compatible compiler for stable
- A C++20-compatible compiler for nightly
- A Windows, Linux, or macOS terminal

## Choose a release channel

### Stable

[`release/stable`](release/stable) is the tested, recommended version for most projects. It currently contains SKCUI v0.1.0 and uses the original C++17 menu API.

### Nightly

[`release/nightly`](release/nightly) tracks the current development header. It adds component-based menus, checkboxes, and `runUi`, but its C++20 API may change while development continues.

Both packages include their own header, setup guide, examples, and license.

## Quick start with stable

Copy `release/stable/src/skcui.hpp` into your project or add `release/stable/src` to your compiler's include path:

```cpp
#include "skcui.hpp"

#include <iostream>
#include <string>
#include <vector>

int main() {
    const std::vector<std::string> options{"Start", "Exit"};
    int selected = 0;

    skcui::menu(selected, options, "Main menu", "Choose an option.");

    std::cout << "Selected: "
              << options[static_cast<std::size_t>(selected)]
              << '\n';
}
```

With an x64 Visual Studio Developer Command Prompt:

```bat
cl /nologo /std:c++17 /EHsc /W4 /I release\stable\src release\stable\examples\menu.cpp /Fe:skcui-example.exe
```

Run the executable, navigate with Up and Down, and press Enter to select.

## Try nightly

Nightly includes separate menu and checkbox examples:

```bat
cl /nologo /std:c++20 /EHsc /W4 /I release\nightly\src release\nightly\examples\menu.cpp /Fe:skcui-nightly-menu.exe
cl /nologo /std:c++20 /EHsc /W4 /I release\nightly\src release\nightly\examples\checkbox.cpp /Fe:skcui-nightly-checkbox.exe
```

See the [nightly guide](release/nightly/README.md) and [nightly API reference](release/nightly/docs/api.md) before using the development API.

## Repository layout

- `dev/` — Visual Studio development solution and working header
- `release/stable/` — tested, self-contained stable distribution
- `release/nightly/` — current development snapshot with examples and API docs
- `docs/` — stable API documentation and release notes

Open `dev/skcui.slnx` for Visual Studio development. See the [`release` index](release/README.md) for a shorter comparison of both channels.

## Documentation

- [Stable API reference](docs/api.md)
- [Nightly API reference](release/nightly/docs/api.md)
- [v0.1.0 release notes](docs/releases/v0.1.0.md)

## Current limitations

- Interactive input is synchronous.
- Stable currently supports menus only; checkboxes are available in nightly.
- Linux and macOS input paths are included but were not verified on this Windows release host.
- Nightly is under active development and may introduce breaking API changes.

## License

SKCUI is available under the [MIT License](LICENSE).
