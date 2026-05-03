#include "HIDMouse.h"
#include "HIDUtils.h"
#include <cstring>

/* ----------------------------------------------- */

HIDMouseData::HIDMouseData() : support(0),
                               x(0),
                               y(0),
                               wheel(0),
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
    this->m_reports = descriptor->get_reports();
}

/* ----------------------------------------------- */

HIDMouse::~HIDMouse()
{
}

/* ----------------------------------------------- */

bool HIDMouse::is_valid()
{
    return get_count() > 0;
}

/* ----------------------------------------------- */

uint8_t HIDMouse::get_count()
{
    uint8_t count = 0;

    for (const auto &report : this->m_reports)
    {
        if (report.report_type == HIDIOReportType::Mouse || report.report_type == HIDIOReportType::Pointer)
            count++;
    }

    return count;
}

/* ----------------------------------------------- */

static int32_t sign_extend(uint32_t value, uint32_t bits)
{
    if (bits == 0 || bits >= 32)
        return (int32_t)value;

    uint32_t sign_bit = 1u << (bits - 1);
    if (value & sign_bit)
        return (int32_t)(value | (~0u << bits));

    return (int32_t)value;
}

/* ----------------------------------------------- */

bool HIDMouse::parse_data(uint8_t *data, uint16_t datalen, HIDMouseData *mouse_data)
{
    bool found = false;
    const uint32_t total_bits = static_cast<uint32_t>(datalen) * 8;

    for (uint32_t i = 0; i < this->m_reports.size(); i++)
    {
        const auto &report = this->m_reports[i];

        if (report.report_type != HIDIOReportType::Mouse && report.report_type != HIDIOReportType::Pointer)
            continue;

        for (const auto &ioblock : report.inputs)
        {
            uint32_t bit_offset = 0;

            for (const auto &input : ioblock.data)
            {
                uint32_t value = HIDUtils::read_bits_le(data, bit_offset, input.size);
                bit_offset += input.size;

                if (bit_offset > total_bits)
                    return false; // Out of range

                if (input.type == HIDIOType::ReportId)
                {
                    if (value != input.id)
                        break; // Not the correct report id
                }

                found = true;

                switch (input.type)
                {
                case HIDIOType::Button:
                    if (input.id >= MAX_MOUSE_BUTTONS)
                        break;
                    mouse_data->buttons[input.id] = value;
                    if (mouse_data->button_count <= input.id)
                        mouse_data->button_count = input.id + 1;
                    break;
                case HIDIOType::X:
                    mouse_data->support |= MOUSE_SUPPORT_X;
                    mouse_data->x = (int16_t)sign_extend(value, input.size);
                    break;
                case HIDIOType::Y:
                    mouse_data->support |= MOUSE_SUPPORT_Y;
                    mouse_data->y = (int16_t)sign_extend(value, input.size);
                    break;
                case HIDIOType::Wheel:
                    mouse_data->support |= MOUSE_SUPPORT_WHEEL;
                    mouse_data->wheel = (int16_t)sign_extend(value, input.size);
                    break;
                default:
                    break;
                }
            }

            if (found)
                return true;
        }
    }

    return false;
}
