#include "HIDMouse.h"
#include "HIDUtils.h"
#include <cstring>

/* ----------------------------------------------- */

HIDMouseData::HIDMouseData() : support(0),
                               X(0),
                               Y(0),
                               Wheel(0),
                               button_count(0)
{
    memset(buttons, 0, sizeof(buttons));
}

/* ----------------------------------------------- */

HIDMouseData::~HIDMouseData()
{
}

/* ----------------------------------------------- */

HIDMouse::HIDMouse(const std::shared_ptr<HIDReportDescriptor> &descriptor)
{
    this->m_reports = descriptor->GetReports();
}

/* ----------------------------------------------- */

HIDMouse::~HIDMouse()
{
}

/* ----------------------------------------------- */

bool HIDMouse::isValid()
{
    return getCount() > 0;
}

/* ----------------------------------------------- */

uint8_t HIDMouse::getCount()
{
    uint8_t count = 0;

    for (auto report : this->m_reports)
    {
        if (report.report_type == HIDIOReportType::Mouse || report.report_type == HIDIOReportType::Pointer)
            count++;
    }

    return count;
}

/* ----------------------------------------------- */

static int32_t signExtend(uint32_t value, uint32_t bits)
{
    if (bits == 0 || bits >= 32)
        return (int32_t)value;

    uint32_t sign_bit = 1u << (bits - 1);
    if (value & sign_bit)
        return (int32_t)(value | (~0u << bits));

    return (int32_t)value;
}

/* ----------------------------------------------- */

bool HIDMouse::parseData(uint8_t *data, uint16_t datalen, HIDMouseData *mouse_data)
{
    bool found = false;

    for (uint32_t i = 0; i < this->m_reports.size(); i++)
    {
        auto report = this->m_reports[i];

        if (report.report_type != HIDIOReportType::Mouse && report.report_type != HIDIOReportType::Pointer)
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

                if (input.type == HIDIOType::Button)
                {
                    if (input.id >= MAX_MOUSE_BUTTONS)
                        continue;

                    mouse_data->buttons[input.id] = value;
                    if (mouse_data->button_count <= input.id)
                        mouse_data->button_count = input.id + 1;
                }
                else if (input.type == HIDIOType::X)
                {
                    mouse_data->support |= MOUSE_SUPPORT_X;
                    mouse_data->X = (int16_t)signExtend(value, input.size);
                }
                else if (input.type == HIDIOType::Y)
                {
                    mouse_data->support |= MOUSE_SUPPORT_Y;
                    mouse_data->Y = (int16_t)signExtend(value, input.size);
                }
                else if (input.type == HIDIOType::Wheel)
                {
                    mouse_data->support |= MOUSE_SUPPORT_Wheel;
                    mouse_data->Wheel = (int16_t)signExtend(value, input.size);
                }
            }

            if (found)
                return true;
        }
    }

    return false;
}
