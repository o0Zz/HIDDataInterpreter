#include "HIDJoystick.h"
#include "HIDUtils.h"
#include <cstring>

/* ----------------------------------------------- */

static int32_t map_value(int32_t value, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/* ----------------------------------------------- */

HIDJoystickData::HIDJoystickData() : index(0xFF),
                                     support(0),
                                     x(0),
                                     y(0),
                                     z(0),
                                     rx(0),
                                     ry(0),
                                     rz(0),
                                     slider(0),
                                     dial(0),
                                     brake(0),
                                     accelerator(0),
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
    this->m_reports = descriptor->get_reports();
}

/* ----------------------------------------------- */

HIDJoystick::~HIDJoystick()
{
}

/* ----------------------------------------------- */

bool HIDJoystick::is_valid()
{
    return get_count() > 0;
}

/* ----------------------------------------------- */

uint8_t HIDJoystick::get_count()
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

bool HIDJoystick::parse_data(uint8_t *data, uint16_t datalen, HIDJoystickData *joystick_data)
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
            uint32_t bit_offset = 0;

            for (auto input : ioblock.data)
            {
                uint32_t value = HIDUtils::read_bits_le(data, bit_offset, input.size);
                bit_offset += input.size;

                if (bit_offset > (datalen * (uint32_t)8))
                    return false; // Out of range

                if (input.type == HIDIOType::ReportId)
                {
                    if (value != input.id)
                        break; // Not the correct report id
                }

                found = true;
                joystick_data->index = joystick_count - 1;

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
                    joystick_data->x = map_value(value, input.logical_min, input.logical_max, -32768, 32767);
                }
                else if (input.type == HIDIOType::Y)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_Y;
                    joystick_data->y = map_value(value, input.logical_min, input.logical_max, -32768, 32767);
                }
                else if (input.type == HIDIOType::Z)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_Z;
                    joystick_data->z = map_value(value, input.logical_min, input.logical_max, -32768, 32767);
                }
                else if (input.type == HIDIOType::Rx)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_RX;
                    joystick_data->rx = map_value(value, input.logical_min, input.logical_max, -32768, 32767);
                }
                else if (input.type == HIDIOType::Ry)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_RY;
                    joystick_data->ry = map_value(value, input.logical_min, input.logical_max, -32768, 32767);
                }
                else if (input.type == HIDIOType::Rz)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_RZ;
                    joystick_data->rz = map_value(value, input.logical_min, input.logical_max, -32768, 32767);
                }
                else if (input.type == HIDIOType::Slider)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_SLIDER;
                    joystick_data->slider = map_value(value, input.logical_min, input.logical_max, -32768, 32767);
                }
                else if (input.type == HIDIOType::Dial)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_DIAL;
                    joystick_data->dial = map_value(value, input.logical_min, input.logical_max, -32768, 32767);
                }
                else if (input.type == HIDIOType::HatSwitch)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_HAT_SWITCH;
                    joystick_data->hat_switch = (HIDJoystickHatSwitch)value;
                }
                else if (input.type == HIDIOType::Brake)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_BRAKE;
                    joystick_data->brake = map_value(value, input.logical_min, input.logical_max, -32768, 32767);
                }
                else if (input.type == HIDIOType::Accelerator)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_ACCELERATOR;
                    joystick_data->accelerator = map_value(value, input.logical_min, input.logical_max, -32768, 32767);
                }
            }

            if (found)
                return true;
        }
    }

    return false;
}
