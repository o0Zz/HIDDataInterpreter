# Introduction
This project aims to provide a platform independent HID report descriptor parser and data parser.
It allows to parse and access data from a USB device in an abstract manner.

# Supported devices
[X] Joystick
[X] Gamepad

# Architecture
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

# Usage

```
#include "HID.h"

uint_8_t *hid_report_data[1024]; //hid report read from USB
//libusb_control_transfer(handle, LIBUSB_ENDPOINT_IN|LIBUSB_RECIPIENT_INTERFACE, LIBUSB_REQUEST_GET_DESCRIPTOR, (LIBUSB_DT_REPORT << 8)|interface_num, 0, hid_report_data, sizeof(hid_report_data), 5000);

HIDReportDescriptor hidReport = HIDReportDescriptor(hid_report_data);
HIDDevice *dev = HIDFactory::createHidDevice(hidReport.getDeviceDesc(i));

if (dev->getType == HIDDeviceType::Joystick)
{
    HIDDeviceJoystick *joystick = std::static_cast<HIDDeviceJoystick>(dev);
    while (True)
    {
        uint_8_t *usb_data = {/*...*/} //Read data from usb
        std::shared_ptr<HIDJoystickData> data = joystick.parse(usb_data);
        cout << "X: " << data->getX() << " Y: " data->getY() << " Btn0: " data->getButton(0)
    }
}
```
More in tests folder

