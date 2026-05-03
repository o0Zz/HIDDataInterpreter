# CLAUDE.md

## Project Overview

HID Data Interpreter — a platform-independent C++ library for parsing USB HID report descriptors and decoding raw HID data (joysticks, gamepads, mice, keyboards). Designed for use on MCUs or environments without OS-level HID support.

## Design

- Keep code simple and modular — no over-engineering
- Designed for embedded environments: avoid dynamic memory allocation (`new`, `malloc`), exceptions, RTTI, and complex C++ features
- Prefer fixed-size arrays and stack allocation over `std::vector` in data structures where sizes are bounded
- Focus on clarity and maintainability over abstraction
- The architecture is layered — each layer can be used independently:
  - `HIDReportDescriptorElements` + `HIDReportDescriptorUsages` are sufficient for basic keyboard/mouse support without needing the full `HIDJoystick`/`HIDMouse`/`HIDKeyboard` parsing layer

## Architecture

4-layer pipeline (bottom-up):

1. **HIDReportDescriptorElements** (`src/HIDReportDescriptorElements.cpp/.h`) — Raw bytes → key/value pairs
2. **HIDReportDescriptorUsages** (`src/HIDReportDescriptorUsages.cpp/.h`) — Key/value → typed usages with properties
3. **HIDReportDescriptor** (`src/HIDReportDescriptor.cpp`, `inc/HIDReportDescriptor.h`) — Usages → structured HIDIOReport objects
4. **HIDJoystick** (`src/HIDJoystick.cpp`, `inc/HIDJoystick.h`) — User-facing: parses raw USB data into HIDJoystickData
5. **HIDMouse** (`src/HIDMouse.cpp`, `inc/HIDMouse.h`) — User-facing: parses raw USB data into HIDMouseData (buttons, X/Y deltas, wheel)
6. **HIDKeyboard** (`src/HIDKeyboard.cpp`, `inc/HIDKeyboard.h`) — User-facing: parses raw USB data into HIDKeyboardData (modifiers, key scan codes)

Utility functions in `src/HIDUtils.cpp/.h` (bit-level reading).

## Build

```bash
cmake -S . -B build
cmake --build build
```

- C++11 standard
- CMake 3.14+
- Google Test (fetched automatically via FetchContent, release-1.11.0)
- Targets: `HIDDataInterpreterLib` (static library), `HIDDataInterpreterTests` (test executable)
- Also has a Nintendo Switch build target (`build/switch/`)

## Test

```bash
cd build
ctest
```

Tests are in `tests/` — each file covers a real controller (PS4, Xbox 360, Stadia, PS1 adapter, etc.) with actual HID report descriptors and data bytes.

## Code Conventions

- C++11, no newer standards
- Public headers in `inc/`, private headers and sources in `src/`
- Classes prefixed with `HID`
- Enums use `enum class`
- Support flags as `#define` bitmasks (e.g., `JOYSTICK_SUPPORT_X`)
- Axis values normalized to int16_t range (-32768 to 32767)
- Max 32 buttons per joystick
- No external dependencies beyond Google Test (test only)
