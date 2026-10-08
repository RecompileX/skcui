# Contributing to SKCUI

Thanks for helping improve **SKCUI — Simple Keyboard Console UI**, a header-only C++ library for keyboard-driven console interfaces.

## Reporting issues

For a bug report, include:

- The package or commit you are using.
- Your operating system, terminal, compiler, and C++ standard.
- A small example that reproduces the problem.
- The keys or actions that trigger it.
- The expected behavior and what actually happens.

For a feature suggestion, describe the use case and show how you would like to use the API.

## Project layout

- dev/skcui.hpp — development header.
- release/stable/ — current component API, requiring C++20 or newer.
- release/stable 1.x/ — C++17 function API.
- docs/api.md — current component API reference.
- release/stable 1.x/docs/api.md — C++17 API reference.
- assets/ — demo assets.

State which API your change affects. Preserve C++17 compatibility when changing the stable 1.x package, and explain any breaking changes to the component API.

## Building examples

From the repository root, build the current stable example with CMake 3.20 or newer and a compiler supporting C++23, as configured by its example project:

```shell
cmake -S release/stable/examples -B build/stable
cmake --build build/stable --config Release
```

Build the C++17 example separately:

```shell
cmake -S "release/stable 1.x/examples" -B build/stable-1x
cmake --build build/stable-1x --config Release
```


Run the resulting skcui executable in an interactive terminal. Its location depends on the CMake generator and build configuration.

To check a development-header change, compile a small example that includes dev/skcui.hpp. Building a release example checks that package's header, rather than the development header.

## Making changes

Keep each contribution focused on one problem or feature. Follow the surrounding code style and retain the header-only design.

- Use the existing skcui namespaces and component naming conventions.
- Include the standard headers your code needs.
- Consider Windows, Linux, and macOS when changing input or console behavior.
- Document changes to controls, public fields, compiler requirements, or API behavior.
- Add or update an example when it helps demonstrate the change.
- Keep generated builds, executables, and IDE files out of the contribution.

## Checking your contribution

Compile the affected header and exercise the behavior you changed. For interactive changes, check the relevant controls: Up/Down navigation, Enter selection or toggling, Escape, text input, and Backspace.

Check relevant edge cases, such as empty option lists, selection boundaries, add-on removal, and progress-bar bounds. If changing shared input or screen-clearing code, also check the existing menus and checkboxes.

Report the operating systems, terminals, and compilers you actually tested. List any platforms or behavior that remain unverified.

For documentation changes, verify that examples match the relevant header and that links, paths, and build commands point to the current repository layout. Keep the main README, release index, package guides, and affected API reference consistent. Use **Simple Keyboard Console UI** as the project's full name.

## Pull requests

Describe the problem, what changes for users, and how you checked it. Include a short before/after example when the API changes, and identify the affected package or development header.

For larger API or architecture changes, open an issue to discuss the approach before investing in a substantial implementation.

Contributions should be compatible with the project's MIT License. Identify any third-party material and its license.
