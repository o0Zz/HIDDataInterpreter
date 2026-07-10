#include "HIDJoystick.h"
#include "HIDUtils.h"
#include <cstring>

/* ----------------------------------------------- */

static int32_t mapValue(int32_t value, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/* ----------------------------------------------- */

// Some devices (e.g. adapters exposing a real two's-complement stick, such as the
// Adaptoid N64 adapter) declare a negative Logical Minimum for an axis. Per the HID
// spec, a negative Logical Minimum means the raw field is two's-complement and must
// be sign-extended from its declared bit length before being used as a signed value.
static int32_t signExtend(uint32_t value, uint32_t bits)
{
    if (bits == 0 || bits >= 32)
        return (int32_t)value;

    uint32_t signBit = 1u << (bits - 1);
    if (value & signBit)
        return (int32_t)(value | (~0u << bits));

    return (int32_t)value;
}

/* ----------------------------------------------- */

HIDJoystickData::HIDJoystickData() : index(0xFF),
                                     support(0),
                                     X(0),
                                     Y(0),
                                     Z(0),
                                     Rx(0),
                                     Ry(0),
                                     Rz(0),
                                     Slider(0),
                                     Dial(0),
                                     Brake(0),
                                     Accelerator(0),
                                     hat_switch(HIDJoystickHatSwitch::NEUTRAL),
                                     button_count(0)
{
    memset(buttons, 0, sizeof(buttons));
}

/* ----------------------------------------------- */

HIDJoystickData::~HIDJoystickData()
{
}

/* ----------------------------------------------- */

HIDJoystick::HIDJoystick(const std::shared_ptr<HIDReportDescriptor> &descriptor)
{
    this->m_reports = descriptor->GetReports();
}

/* ----------------------------------------------- */

HIDJoystick::~HIDJoystick()
{
}

/* ----------------------------------------------- */

bool HIDJoystick::isValid()
{
    return getCount() > 0;
}

/* ----------------------------------------------- */

uint8_t HIDJoystick::getCount()
{
    uint8_t count = 0;

    for (auto report : this->m_reports)
    {
        if (report.report_type == HIDIOReportType::Joystick || report.report_type == HIDIOReportType::GamePad)
            count++;
    }

    return count;
}

/* ----------------------------------------------- */

bool HIDJoystick::parseData(uint8_t *data, uint16_t datalen, HIDJoystickData *joystick_data)
{
    bool found = false;
    uint8_t joystick_count = 0;

    for (uint32_t i = 0; i < this->m_reports.size(); i++)
    {
        auto report = this->m_reports[i];

        if (report.report_type != HIDIOReportType::Joystick && report.report_type != HIDIOReportType::GamePad)
            continue;

        joystick_count += 1;

        for (auto ioblock : report.inputs)
        {
            uint32_t bitOffset = 0;

            for (auto input : ioblock.data)
            {
                uint32_t value = HIDUtils::readBitsLE(data, bitOffset, input.size);
                bitOffset += input.size;

                if (bitOffset > (datalen * (uint32_t)8))
                    return false; // Out of range

                if (input.type == HIDIOType::ReportId)
                {
                    if (value != input.id)
                        break; // Not the correct report id
                }

                found = true;
                joystick_data->index = joystick_count - 1;

                // A negative Logical Minimum means this field is two's-complement; sign-extend it
                // from its declared bit length before treating it as a signed quantity. Fields with
                // a non-negative Logical Minimum (the common case) are left as a plain magnitude.
                int32_t signedValue = (input.logical_min < 0) ? signExtend(value, input.size) : (int32_t)value;

                if (input.type == HIDIOType::Button)
                {
                    if (input.id >= MAX_BUTTONS)
                        return false;

                    joystick_data->buttons[input.id] = value;
                    if (joystick_data->button_count <= input.id)
                        joystick_data->button_count = input.id + 1;
                }
                if (input.type == HIDIOType::Consumer)
                {
                    joystick_data->consumer_buttons[static_cast<HIDIOConsumerType>(input.id)] = value;
                }
                else if (input.type == HIDIOType::X)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_X;
                    joystick_data->X = mapValue(signedValue, input.logical_min, input.logical_max, -32768, 32767);
                }
                else if (input.type == HIDIOType::Y)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_Y;
                    joystick_data->Y = mapValue(signedValue, input.logical_min, input.logical_max, -32768, 32767);
                }
                else if (input.type == HIDIOType::Z)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_Z;
                    joystick_data->Z = mapValue(signedValue, input.logical_min, input.logical_max, -32768, 32767);
                }
                else if (input.type == HIDIOType::Rx)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_Rx;
                    joystick_data->Rx = mapValue(signedValue, input.logical_min, input.logical_max, -32768, 32767);
                }
                else if (input.type == HIDIOType::Ry)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_Ry;
                    joystick_data->Ry = mapValue(signedValue, input.logical_min, input.logical_max, -32768, 32767);
                }
                else if (input.type == HIDIOType::Rz)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_Rz;
                    joystick_data->Rz = mapValue(signedValue, input.logical_min, input.logical_max, -32768, 32767);
                }
                else if (input.type == HIDIOType::Slider)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_Slider;
                    joystick_data->Slider = mapValue(signedValue, input.logical_min, input.logical_max, -32768, 32767);
                }
                else if (input.type == HIDIOType::Dial)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_Dial;
                    joystick_data->Dial = mapValue(signedValue, input.logical_min, input.logical_max, -32768, 32767);
                }
                else if (input.type == HIDIOType::HatSwitch)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_HatSwitch;
                    joystick_data->hat_switch = (HIDJoystickHatSwitch)value;
                }
                else if (input.type == HIDIOType::Brake)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_Brake;
                    joystick_data->Brake = mapValue(signedValue, input.logical_min, input.logical_max, -32768, 32767);
                }
                else if (input.type == HIDIOType::Accelerator)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_Accelerator;
                    joystick_data->Accelerator = mapValue(signedValue, input.logical_min, input.logical_max, -32768, 32767);
                }
            }

            if (found)
                return true;
        }
    }

    return false;
}
