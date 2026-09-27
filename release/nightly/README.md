# SKCUI Nightly

> Warning: Nightly tracks the current development header. Its API may change and it may contain unfinished behavior. Use `../stable` for production projects.

Nightly is header-only and currently requires C++20 because its component API uses concepts.

## Install

Copy `src/skcui.hpp` into your project or add `src` to the compiler include path:

```cpp
#include "skcui.hpp"
```

## Examples

Build from this directory in an x64 Visual Studio Developer Command Prompt:

```bat
cl /nologo /std:c++20 /EHsc /W4 /I src examples\menu.cpp /Fe:examples\skcui-menu.exe
cl /nologo /std:c++20 /EHsc /W4 /I src examples\checkbox.cpp /Fe:examples\skcui-checkbox.exe
```

- Menu: Up/Down moves; Enter confirms.
- Checkbox: Up/Down moves; Enter toggles; Escape closes.

See `docs/api.md` for the development API.
