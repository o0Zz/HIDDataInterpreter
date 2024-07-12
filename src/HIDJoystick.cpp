#include "HIDJoystick.h"
#include "HIDUtils.h"
#include <cstring>

/* ----------------------------------------------- */

static int32_t mapValue(int32_t value, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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

    for (uint32_t i = 0; i < this->m_reports.size(); i++)
    {
        auto report = this->m_reports[i];

        if (report.report_type != HIDIOReportType::Joystick && report.report_type != HIDIOReportType::GamePad)
            continue;

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
                joystick_data->index = i;

                if (input.type == HIDIOType::Button)
                {
                    if (input.id >= MAX_BUTTONS)
                        return false;

                    joystick_data->buttons[input.id] = value;
                    if (joystick_data->button_count < input.id)
                        joystick_data->button_count = input.id;
                }
                else if (input.type == HIDIOType::X)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_X;
                    joystick_data->X = mapValue(value, input.logical_min, input.logical_max, -32768, 32767);
                }
                else if (input.type == HIDIOType::Y)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_Y;
                    joystick_data->Y = mapValue(value, input.logical_min, input.logical_max, -32768, 32767);
                }
                else if (input.type == HIDIOType::Z)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_Z;
                    joystick_data->Z = mapValue(value, input.logical_min, input.logical_max, -32768, 32767);
                }
                else if (input.type == HIDIOType::Rx)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_Rx;
                    joystick_data->Rx = mapValue(value, input.logical_min, input.logical_max, -32768, 32767);
                }
                else if (input.type == HIDIOType::Ry)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_Ry;
                    joystick_data->Ry = mapValue(value, input.logical_min, input.logical_max, -32768, 32767);
                }
                else if (input.type == HIDIOType::Rz)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_Rz;
                    joystick_data->Rz = mapValue(value, input.logical_min, input.logical_max, -32768, 32767);
                }
                else if (input.type == HIDIOType::HatSwitch)
                {
                    joystick_data->support |= JOYSTICK_SUPPORT_HatSwitch;
                    joystick_data->hat_switch = (HIDJoystickHatSwitch)value;
                }
            }

            if (found)
                return true;
        }
    }

    return false;
}
