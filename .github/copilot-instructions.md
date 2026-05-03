# Copilot Instructions

## Project

HID Data Interpreter — a platform-independent C++ library for parsing USB HID report descriptors and decoding raw HID data for joysticks, gamepads, mice, and keyboards. Runs on MCUs and systems without OS-level HID support.

## Design

- Keep code simple and modular — no over-engineering
- Designed for embedded environments: avoid dynamic memory allocation (`new`, `malloc`), exceptions, RTTI, and complex C++ features
- Prefer fixed-size arrays and stack allocation over `std::vector` in data structures where sizes are bounded
- Focus on clarity and maintainability over abstraction
- The architecture is layered — each layer can be used independently:
  - `HIDReportDescriptorElements` + `HIDReportDescriptorUsages` are sufficient for basic keyboard/mouse support without needing the full `HIDJoystick`/`HIDMouse`/`HIDKeyboard` parsing layer

## Code Style

- C++11 standard (no C++14/17/20 features)
- Public headers in `inc/`, implementation and private headers in `src/`
- Class names prefixed with `HID` (e.g., `HIDJoystick`, `HIDReportDescriptor`)
- Use `enum class` for enumerations
- Support flags as `#define` bitmasks
- Axis values normalized to `int16_t` range (-32768 to 32767)
- Use `uint8_t`, `uint16_t`, `int32_t` etc. from `<stdint.h>`
- No external runtime dependencies; Google Test for tests only
- Separate comment dividers (`/* --- */`) between function implementations

## Architecture

Pipeline: raw descriptor bytes → `HIDReportDescriptorElements` → `HIDReportDescriptorUsages` → `HIDReportDescriptor` → `HIDJoystick`

- `HIDReportDescriptorElements` — parses raw bytes into key/value elements
- `HIDReportDescriptorUsages` — converts elements into typed usages
- `HIDReportDescriptor` — builds `HIDIOReport` with input/output/feature blocks
- `HIDJoystick` — decodes raw USB data into `HIDJoystickData` using parsed reports
- `HIDMouse` — decodes raw USB data into `HIDMouseData` (buttons, X/Y deltas, wheel)
- `HIDKeyboard` — decodes raw USB data into `HIDKeyboardData` (modifiers, key scan codes)
- `HIDUtils` — bit-level data reading utilities

## Build System

- CMake 3.14+
- Library target: `HIDDataInterpreterLib`
- Test target: `HIDDataInterpreterTests` (Google Test, fetched via FetchContent)

## Testing

- Each test file covers a real controller with actual HID descriptors and data
- Test files named `test_joystick_<controller>.cpp`, `test_mouse.cpp`, or `test_keyboard.cpp`
- Use Google Test (`gtest`) assertions
