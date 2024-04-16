#pragma once
#include <stdint.h>
#include <memory>
#include <vector>
#include <string>

enum class HIDInputType {
    Unknown = 0, 
    ReportID,
    Button, 
    X, 
    Y,
    HatSwitch,
    Padding
};
    
class HIDInput
{
public:
    HIDInput() : 
        type(HIDInputType::Unknown), 
        logical_min(0), 
        logical_max(0), 
        physical_min(0), 
        physical_max(0), 
        unit(0), 
        unit_exponent(0), 
        size(0), 
        count(0) {}
    ~HIDInput() {}

    HIDInputType type; //Input type (ReportID, Button, X, Y, Hat switch, Padding, etc.)
    uint32_t logical_min;
    uint32_t logical_max;
    uint32_t physical_min;
    uint32_t physical_max;
    uint32_t unit;
    uint32_t unit_exponent;
    uint32_t size; //Size of the data in bits
    uint32_t count; //Number of data items

    bool isComplete() const {
        return size != 0;
    }
};
class HIDEntry;

class HIDReportDescriptor
{
public:
    HIDReportDescriptor(const uint8_t *hid_report_data, uint16_t hid_report_data_size);
    ~HIDReportDescriptor();

private:
    void                                        parse(const uint8_t *hid_report_data, uint16_t hid_report_data_len);
    std::vector<std::shared_ptr<HIDEntry>>  parseKeyValues(const uint8_t *hid_report_data, uint16_t hid_report_data_len);

    std::vector<HIDInput> m_inputs;
};
