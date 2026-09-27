# SKCUI Nightly API

Nightly reflects the development header and requires C++20.

## Components

### `skcui::component::menu`

```cpp
skcui::component::menu menu{
    0,
    options,
    "Optional title",
    "Optional description"
};
```

`selected` contains the chosen zero-based index after Enter is pressed. `options` must remain alive while the component is used and should not be empty.

### `skcui::component::checkbox`

```cpp
skcui::component::checkbox choices{
    {},
    {{"First", false}, {"Second", true}},
    0
};
```

Each pair stores its label and checked state. Enter toggles the highlighted value; Escape exits. The checkbox list should not be empty.

## `skcui::runUi`

```cpp
skcui::runUi(component1, component2);
```

Renders one or more supported component lvalues in order. Components are updated in place.

## Input helpers

`skcui::getKey()` normalizes Up, Down, Enter, and Escape. `skcui::clearScreen()` clears the active console between renders.
