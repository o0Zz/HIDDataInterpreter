#include "HIDUtils.h"

uint32_t HIDUtils::readBits(uint8_t *buffer, uint32_t bitOffset, uint32_t bitLength) {
    // Calculate the starting byte index and bit index within that byte
    uint32_t byteIndex = bitOffset / 8;
    uint32_t bitIndex = 7 - (bitOffset % 8);  // Big endian, MSB is at index 7

    uint32_t result = 0;

    for (uint32_t i = 0; i < bitLength; ++i) {
        // Check if we need to move to the next byte
        if (bitIndex > 7) {
            ++byteIndex;
            bitIndex = 7;
        }

        // Get the bit at the current position and add it to the result
        uint8_t bit = (buffer[byteIndex] >> bitIndex) & 0x01;
        result |= (bit << (bitLength - 1 - i));

        // Move to the next bit
        --bitIndex;
    }

    return result;
}
