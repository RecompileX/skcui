# SKCUI Stable

This is the tested, recommended SKCUI release channel. It currently contains v0.1.0.

## Install

Copy `src/skcui.hpp` into your project or add `src` to your compiler include path. SKCUI stable requires C++17 or newer.

```cpp
#include "skcui.hpp"
```

## Example

Build the menu example from this directory in an x64 Visual Studio Developer Command Prompt:

```bat
cl /nologo /std:c++17 /EHsc /W4 /I src examples\menu.cpp /Fe:examples\skcui-menu.exe
```

Use Up and Down to navigate and Enter to choose an item.

## Channel policy

Stable changes only when a tested release is promoted. For the newest in-development API, see `../nightly`.
