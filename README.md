# HID Data Interpreter

A platform-independent C++ library for parsing USB HID report descriptors and decoding raw HID data. Supports joysticks, gamepads, mice, and keyboards without requiring OS-level HID support — ideal for MCUs and embedded environments.

## Features

- Parses raw HID report descriptors into structured data
- Decodes USB HID input data for multiple device types
- No dynamic memory allocation in hot paths — suitable for embedded use
- No external runtime dependencies
- C++11 compatible

## Supported Devices

- [x] Joystick / Gamepad (axes, buttons, hat switch, sliders, triggers)
- [x] Mouse (buttons, X/Y deltas, scroll wheel)
- [x] Keyboard (modifier keys, scan codes)

## Building

Requires CMake 3.14+ and a C++11 compiler.

```bash
cmake -S . -B build
cmake --build build
```

### Running Tests

```bash
cd build
ctest
```

Tests cover real controllers including PS4, Xbox 360, Google Stadia, PS1 adapters, and more.

## Architecture

The library uses a layered pipeline — each layer can be used independently:

```
Raw HID Descriptor bytes
         │
         ▼
┌─────────────────────────────┐
│ HIDReportDescriptorElements │  Parses raw bytes into key/value elements
└──────────────┬──────────────┘
               │
               ▼
┌─────────────────────────────┐
│  HIDReportDescriptorUsages  │  Converts elements into typed usages
└──────────────┬──────────────┘
               │
               ▼
┌─────────────────────────────┐
│     HIDReportDescriptor     │  Builds structured HIDIOReport objects
└──────────────┬──────────────┘
               │
               ▼
┌─────────────────────────────┐
│  HIDJoystick / HIDMouse /   │  Decodes raw USB data into device-specific
│       HIDKeyboard           │  data structures
└─────────────────────────────┘
```

## Usage

### Joystick

```cpp
#include "HIDReportDescriptor.h"
#include "HIDJoystick.h"

// Parse the HID report descriptor (obtained from the device)
uint8_t hid_report_data[1024];
uint16_t report_size = /* read descriptor from USB device */;

auto report_desc = std::make_shared<HIDReportDescriptor>(hid_report_data, report_size);
HIDJoystick joystick(report_desc);

if (joystick.is_valid())
{
    HIDJoystickData joystick_data;
    uint8_t usb_data[64]; // raw USB interrupt transfer data

    if (joystick.parse_data(usb_data, sizeof(usb_data), &joystick_data))
    {
        // Access normalized axis values (-32768 to 32767)
        int16_t x = joystick_data.x;
        int16_t y = joystick_data.y;

        // Access buttons 
        uint8_t btn1 = joystick_data.buttons[1];

        // Check hat switch
        HIDJoystickHatSwitch hat = joystick_data.hat_switch;
    }
}
```

### Mouse

```cpp
#include "HIDReportDescriptor.h"
#include "HIDMouse.h"

auto report_desc = std::make_shared<HIDReportDescriptor>(hid_report_data, report_size);
HIDMouse mouse(report_desc);

if (mouse.is_valid())
{
    HIDMouseData mouse_data;

    if (mouse.parse_data(usb_data, sizeof(usb_data), &mouse_data))
    {
        int16_t dx = mouse_data.x;      // relative X movement
        int16_t dy = mouse_data.y;      // relative Y movement
        int16_t wheel = mouse_data.wheel; // scroll wheel delta
        uint8_t left_btn = mouse_data.buttons[0];
    }
}
```

### Keyboard

```cpp
#include "HIDReportDescriptor.h"
#include "HIDKeyboard.h"

auto report_desc = std::make_shared<HIDReportDescriptor>(hid_report_data, report_size);
HIDKeyboard keyboard(report_desc);

if (keyboard.is_valid())
{
    HIDKeyboardData keyboard_data;

    if (keyboard.parse_data(usb_data, sizeof(usb_data), &keyboard_data))
    {
        uint8_t mods = keyboard_data.modifiers; // KEYBOARD_MODIFIER_* bitmask
        for (int i = 0; i < keyboard_data.key_count; i++)
        {
            uint8_t scancode = keyboard_data.keys[i]; // USB HID scan code
        }
    }
}
```

