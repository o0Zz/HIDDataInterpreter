#include "HIDReportDescriptorElements.h"

//https://docs.kernel.org/hid/hidreport-parsing.html
#define HID_FUNC_TYPE_MASK 0xFC
#define HID_TYPE_MASK 0x0C
#define HID_LENGTH_MASK 0x03

/* -------------------------------------------------------------------------- */

HIDElement::HIDElement() :
    type(HIDElementType::HID_UNKNOWN),
    data_size(0)
{
    memset(data, 0x00, sizeof(data));
}

/* -------------------------------------------------------------------------- */

HIDElement::~HIDElement() 
{
}

/* -------------------------------------------------------------------------- */

HIDElement::HIDElement(HIDElementType type, const uint8_t *data, uint8_t data_size)
{
    this->type = type;
    memset(this->data, 0x00, sizeof(this->data));
    memcpy(this->data, data, data_size);
    this->data_size = data_size;
}

/* -------------------------------------------------------------------------- */

HIDElementType HIDElement::GetType() const
{
    return type;
}

/* -------------------------------------------------------------------------- */

uint32_t HIDElement::GetValueUint32() const
{
    return data[0] | ((uint32_t)data[1] << 8) | ((uint32_t)data[2] << 16) | ((uint32_t)data[3] << 24);
}

/* -------------------------------------------------------------------------- */

std::vector<HIDElement> HIDReportDescriptorElements::parse(const uint8_t *hid_report_data, uint16_t hid_report_data_len)
{
    std::vector<HIDElement> elements;

    for (uint16_t offset = 0; offset < hid_report_data_len; /* ... */)
    {
        uint8_t type = hid_report_data[offset];
        uint8_t datalen = type & HID_LENGTH_MASK;
        if (datalen == 3) datalen = 4;

        elements.push_back(HIDElement((HIDElementType)(type & HID_FUNC_TYPE_MASK), &hid_report_data[offset + 1], datalen));

        offset += 1 + datalen;
    }

    return elements;  
}