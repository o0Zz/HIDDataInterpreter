#include "HIDKeyboard.h"
#include "HIDUtils.h"
#include <cstring>

/* ----------------------------------------------- */

HIDKeyboardData::HIDKeyboardData() : modifiers(0),
                                     key_count(0)
{
    memset(keys, 0, sizeof(keys));
}

/* ----------------------------------------------- */

HIDKeyboardData::~HIDKeyboardData()
{
}

/* ----------------------------------------------- */

HIDKeyboard::HIDKeyboard(const std::shared_ptr<HIDReportDescriptor> &descriptor)
{
    this->m_reports = descriptor->GetReports();
}

/* ----------------------------------------------- */

HIDKeyboard::~HIDKeyboard()
{
}

/* ----------------------------------------------- */

bool HIDKeyboard::is_valid()
{
    return get_count() > 0;
}

/* ----------------------------------------------- */

uint8_t HIDKeyboard::get_count()
{
    uint8_t count = 0;

    for (auto report : this->m_reports)
    {
        if (report.report_type == HIDIOReportType::Keyboard || report.report_type == HIDIOReportType::Keypad)
            count++;
    }

    return count;
}

/* ----------------------------------------------- */

bool HIDKeyboard::parse_data(uint8_t *data, uint16_t datalen, HIDKeyboardData *keyboard_data)
{
    bool found = false;

    for (uint32_t i = 0; i < this->m_reports.size(); i++)
    {
        auto report = this->m_reports[i];

        if (report.report_type != HIDIOReportType::Keyboard && report.report_type != HIDIOReportType::Keypad)
            continue;

        for (auto ioblock : report.inputs)
        {
            uint32_t bit_offset = 0;
            uint8_t modifier_index = 0;
            uint8_t key_index = 0;

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

                if (input.type == HIDIOType::Key)
                {
                    // Modifier keys (Usage IDs 0xE0-0xE7) are 1-bit each
                    if (input.size == 1 && input.id >= 0xE0 && input.id <= 0xE7)
                    {
                        if (value)
                            keyboard_data->modifiers |= (1 << (input.id - 0xE0));
                    }
                    // Key scan codes (typically 8-bit usage values)
                    else if (input.size == 8 && value != 0)
                    {
                        if (key_index < MAX_KEYBOARD_KEYS)
                        {
                            keyboard_data->keys[key_index] = (uint8_t)value;
                            key_index++;
                            keyboard_data->key_count = key_index;
                        }
                    }
                }
            }

            if (found)
                return true;
        }
    }

    return false;
}
