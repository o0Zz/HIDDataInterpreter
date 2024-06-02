# HID Data Interpreter 

#### A HID Report Descriptor parser with data parsing (Joystick, Gamepad, Keyboard, Mouse)

## Description
  This project aims to provide a platform independent HID report descriptor parser and HID data parser.
  It allows to decode data from an USB HID device without the need to have an operating system.
  Usually, this parsing and decoding are applied by the operating system at low level.
  However if you need to parse HID data from MCU or for any other purpose, this library might be helpful.


## Supported devices

- [X] Joystick
- [X] Gamepad

## Building (For developers)

Use CMAKE in cmd line or vscode with CMAKE plugin and TestMate.

```
cmake . build
cd build
make
```

### Architecture
```
                                                    
                   ┌─────────────────────────────┐
      Input        │                             │   Output
                   │                             │
──────────────────►│         HIDJoystick         ├─────────────►   HIDJoystickData
                   │                             │
Joystick Raw data  │                             │
                   └─────────────────────────────┘
                                 ▲                
                                 │                
                                 │                
                   ┌─────────────┴───────────────┐
                   │                             │
                   │                             │
                   │     HIDReportDescriptor     │ Responsible to convert Usages to
                   │                             │ HIDInputOutput blocks
                   │                             │
                   └─────────────────────────────┘
                                 ▲                
                                 │                
                                 │                
                   ┌─────────────┴───────────────┐
                   │                             │
                   │                             │
                   │  HIDReportDescriptorUsages  │ Responsible to convert Key/Value to
                   │                             │ Usages with properties
                   │                             │
                   └─────────────────────────────┘
                                 ▲                
                                 │                
                                 │                
                   ┌─────────────┴───────────────┐
                   │                             │
                   │                             │
                   │ HIDReportDescriptorElements │ Responsible to convert report to
                   │                             │ Key/Value
                   │                             │
                   └─────────────────────────────┘
                                 ▲                
                                 │                
                                 │ Input          
                                 │                
                      Raw HID Descriptor report 
				 
```
https://asciiflow.com/

### Usage

```
    #include "HIDReportDescriptor.h"
    #include "HIDJoystick.h"

    HIDJoystickData joystick_data;
    uint_8_t *hid_report_data[1024]; //RAW hid report read from USB

        //Read hid report descriptor using libusb
    int report_size = libusb_control_transfer(handle, LIBUSB_ENDPOINT_IN|LIBUSB_RECIPIENT_INTERFACE, LIBUSB_REQUEST_GET_DESCRIPTOR, (LIBUSB_DT_REPORT << 8)|interface_num, 0, hid_report_data, sizeof(hid_report_data), 5000);

    if (report_size > 0)
    {
        std::shared_ptr<HIDReportDescriptor> report_desc = std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data));

        HIDJoystick joystick(report_desc);

	    
        while (True)
        {
            uint_8_t *usb_data = {/*...*/} //Read data from usb

            if ( joystick.parseData(data, sizeof(data), &joystick_data) )
            {
                cout << "X: " << joystick_data.X << " Y: " joystick_data.Y << " Btn1: " joystick_data.buttons[1]
            }
        }
    }
}
```
More in tests folder

