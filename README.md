# Introduction
This project aims to provide a platform independent HID report descriptor parser and build a data parser allowing to properly parse data from the peripheral in order to access to them easily and in an abstract way.

# Supported devices
[X] Joysticks

# How to use the library

```
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

