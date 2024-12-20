#include "io.hpp" 

namespace io { 
    const uint8_t POLYNOM = 0b10001001; // x^7 + x^3 + 1
    const uint8_t CRC7MASK = 0b01111111; 

    uint8_t crc8_calc(const std::vector<bool>& inputData) {
        uint8_t crc = 0;

        for (bool bit : inputData) {
            uint8_t topBit = (crc & 0x40) >> 6; 
            crc = ((crc << 1) & CRC7MASK) | (bit ? 1 : 0);
            if (topBit) {
                crc ^= POLYNOM;
            }
        }

        for (int i = 0; i < 7; ++i) {
            uint8_t topBit = (crc & 0x40) >> 6;
            crc = (crc << 1) & CRC7MASK;
            if (topBit) {
                crc ^= POLYNOM;
            }
        }

        return crc;
    }

    bool crc8_check(const std::vector<bool>& inputWithCrc) {
        if (inputWithCrc.size() == 1) { 
            return false;
        }

        uint8_t resultCrc = crc8_calc(inputWithCrc);
        return resultCrc == 0;
    }
}