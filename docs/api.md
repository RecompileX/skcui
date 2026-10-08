# SKCUI API Reference

**SKCUI** stands for **Simple Keyboard Console UI**. This reference describes the component API in `release/stable/src/skcui.hpp` and `dev/skcui.hpp`.

The header requires C++20 or newer. Include `skcui.hpp`; no separate library link step is required. The bundled stable example uses C++23 in its CMake configuration. For the C++17 function API, see the [stable 1.x reference](../release/stable%201.x/docs/api.md).

## Menus and checkboxes

```cpp
skcui::component::Menu menu{{"Start", "Exit"}};
skcui::display::render(menu);

skcui::component::Checkbox settings{{
    {"Sound", true},
    {"Animations", false}
}};
skcui::display::render(settings);
```

Both components own their option lists and inherit from `component::Component`.

| Component | State | Controls |
| --- | --- | --- |
| `Menu` | `options` is a const vector of strings; `selected` is a zero-based index, initially 0. | Up/Down moves; Enter on an option returns; Escape returns. |
| `Checkbox` | `checkboxName` is a vector of label/bool pairs; `selected` is a zero-based index, initially 0. | Up/Down moves; Enter on an option toggles its bool; Escape returns. |

Pass a mutable component to `skcui::display::render`. Rendering is synchronous and updates the component in place. Call it once for each component you want to display. A component with no options and no add-ons returns immediately.

A menu's `selected` value alone does not distinguish Enter from Escape. If input add-ons are present, the selected index may refer to an input rather than an option; check it against the option count before indexing.

## Add-on components

Use `add` to append components below the options:

```cpp
menu.add(skcui::component::Text{"Choose an option."});
menu.add(skcui::component::Separator{'-', 24});
menu.add(skcui::component::BlankSeparator{1});
menu.add(skcui::component::Input{"", "Name: "});
menu.add(skcui::component::ProgressBar{'#', 70, 0, 100, 20});
```

| Type | Fields and defaults |
| --- | --- |
| `Text` | `text = " "` |
| `Input` | `value` starts empty; set `prompt` explicitly, or use `"- 1"` to hide it. |
| `Separator` | `symbol = '-'`, `width = 20` |
| `BlankSeparator` | `separatorAmount = 1` (number of newline characters) |
| `ProgressBar` | `symbol = '*'`, `progress = 0`, `min = 0`, `max = 0`, `width = 20` |

Add-ons are stored by value in `addOn`, a vector of `Component::Child` variants. Editing the original object after `add` does not update the stored copy. Retrieve input after rendering through the stored variant:

```cpp
skcui::component::Menu form{{"Submit"}};
form.add(skcui::component::Input{"", "Name: "});
skcui::display::render(form);
const auto& name = std::get<skcui::component::Input>(form.addOn[0]).value;
```

Up/Down navigation includes input add-ons after the main options. Printable characters append to the focused input; Backspace removes its last character. Enter on an input leaves the UI open.

`remove(index)` erases an add-on at a valid zero-based index; an invalid index has no effect. `remove()` removes the last add-on if present. Set `symbol` on a menu or checkbox to change its selection marker (default `'>'`).

## Standalone progress bars

```cpp
std::string skcui::progressBar(
    float progress,
    float min,
    float max,
    std::optional<char> symbol,
    std::optional<int> width);
```

Returns a brace-enclosed bar, with progress clamped to the range. Pass `std::nullopt` for `symbol` to use `'*'` or for `width` to use 10. Both optional arguments must still be supplied. Use a nonnegative width and an increasing range; equal bounds produce `"{}"`. A `component::ProgressBar` uses the same range behavior when rendered.

```cpp
std::cout << skcui::progressBar(50, 0, 100, '#', 20);
```

## Keyboard input

```cpp
char skcui::getKey();
```

Waits for keyboard input using `_getch` on Windows and terminal input APIs on Linux/macOS.

| Constant | Meaning |
| --- | --- |
| `KEY_UP` | Up arrow |
| `KEY_DOWN` | Down arrow |
| `KEY_ENTER` | Enter |
| `KEY_ESC` | Escape |
| `KEY_BACKSPACE` | Backspace |
| `KEY_NONE` | No recognized event or an input failure |

Other characters are returned for text input. Linux/macOS escape-sequence parsing uses a short timeout to distinguish an arrow sequence from Escape.

## Clearing the console

```cpp
void skcui::clearScreen();
```

By default, writes ANSI sequences to move to the top-left corner and clear the screen and scrollback. Define `SKCUILEGACYCONSOLE` before including the header to use `system("cls")` on Windows or `system("clear")` elsewhere.