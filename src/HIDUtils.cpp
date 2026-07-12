#include "HIDUtils.h"

uint32_t HIDUtils::read_bits_le(uint8_t *buffer, uint32_t bit_offset, uint32_t bit_length) {
    // Calculate the starting byte index and bit index within that byte
    uint32_t byte_index = bit_offset / 8;
    uint32_t bit_index = bit_offset % 8;  // Little endian, LSB is at index 0

    uint32_t result = 0;

    for (uint32_t i = 0; i < bit_length; ++i) {
        // Check if we need to move to the next byte
        if (bit_index > 7) {
            ++byte_index;
            bit_index = 0;
        }

        // Get the bit at the current position and add it to the result
        uint8_t bit = (buffer[byte_index] >> bit_index) & 0x01;
        result |= (bit << i);

        // Move to the next bit
        ++bit_index;
    }

    return result;
}


/* ----------------------------------------------- */

int32_t HIDUtils::sign_extend(uint32_t value, uint32_t bits)
{
    if (bits == 0 || bits >= 32)
        return (int32_t)value;

    uint32_t sign_bit = 1u << (bits - 1);
    if (value & sign_bit)
        return (int32_t)(value | (~0u << bits));

    return (int32_t)value;
}
