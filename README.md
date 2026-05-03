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

```cpp
#include <cstdio>
#include <memory>
#include <libusb-1.0/libusb.h>
#include "HIDReportDescriptor.h"
#include "HIDJoystick.h"
#include "HIDMouse.h"
#include "HIDKeyboard.h"

int main()
{
    libusb_context *ctx = NULL;
    libusb_device_handle *handle = NULL;
    uint8_t usb_data[64];
    uint8_t hid_report_data[1024];
    int transferred = 0;
    int interface_num = 0;

    libusb_init(&ctx);

    // Open your USB device (vendor_id, product_id)
    handle = libusb_open_device_with_vid_pid(ctx, 0x054C, 0x09CC); // e.g. PS4 controller
    if (!handle)
        return -1;

    libusb_claim_interface(handle, 0);

    // Get HID report descriptor
    int report_size = libusb_control_transfer(handle, LIBUSB_ENDPOINT_IN | LIBUSB_RECIPIENT_INTERFACE, LIBUSB_REQUEST_GET_DESCRIPTOR, (LIBUSB_DT_REPORT << 8) | interface_num, interface_num, hid_report_data, sizeof(hid_report_data, 5000);

    if (report_size < 0)
        return -1;

    // Parse the HID report descriptor
    auto report_desc = std::make_shared<HIDReportDescriptor>(hid_report_data, (uint16_t)report_size);

    // Read raw USB interrupt data
    libusb_interrupt_transfer(handle, 0x81 /*Typically first IN endpoint*/, usb_data, sizeof(usb_data), &transferred, 5000);

    uint16_t types = report_desc->get_device_types();
    if (types & (HID_DEVICE_JOYSTICK | HID_DEVICE_GAMEPAD))
    {
        HIDJoystick joystick(report_desc);
        HIDJoystickData data;

        if (joystick.parse_data(usb_data, (uint16_t)transferred, &data))
        {
            printf("Joystick: X=%d Y=%d btn0=%d\n", data.x, data.y, data.buttons[0]);
        }
    }

    if (types & HID_DEVICE_MOUSE)
    {
        HIDMouse mouse(report_desc);
        HIDMouseData data;

        if (mouse.parse_data(usb_data, (uint16_t)transferred, &data))
        {
            printf("Mouse: dX=%d dY=%d wheel=%d\n", data.x, data.y, data.wheel);
        }
    }

    if (types & HID_DEVICE_KEYBOARD)
    {
        HIDKeyboard keyboard(report_desc);
        HIDKeyboardData data;

        if (keyboard.parse_data(usb_data, (uint16_t)transferred, &data))
        {
            printf("Keyboard: mods=0x%02X keys=%d\n", data.modifiers, data.key_count);
        }
    }

    libusb_release_interface(handle, 0);
    libusb_close(handle);
    libusb_exit(ctx);
    return 0;
}
```

