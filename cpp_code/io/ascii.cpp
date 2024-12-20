#include "io.hpp"

namespace io {

    std::vector<bool> ascii_encode_char(char c) {
        std::vector<bool> bits(8);
        
        for (int i = 0; i < 8; ++i) {
            bits[7 - i] = (c & (1 << i)) != 0;
        }

        return bits;
    }

    char ascii_decode_char(const std::vector<bool>& bits) {
        if (bits.size() != 8) {
            throw std::invalid_argument("ascii_decode_char(); The size of the bit vector must be equal of 8.");
        }

        char c = 0;

        for (size_t i = 0; i < 8; ++i) {
            if (bits[i]) {
                c |= (1 << (7 - i));
            }
        }
        return c;
    }

    std::vector<bool> ascii_encode_string(const std::string& input) {
        std::vector<bool> encoded;

        for (char c : input) { 
            std::vector<bool> ascii_bit = ascii_encode_char(c); 
            encoded.insert(encoded.end(), ascii_bit.begin(), ascii_bit.end()); 
        }

        return encoded; 
    }

    std::string ascii_decode_string(const std::vector<bool>& bitVector) { 
        if (bitVector.size() % 8 != 0) { 
            throw std::invalid_argument("ascii_decode_string(); The size of the bit vector must be a multiple of 8.");
        }

        std::string decoded; 

        for (size_t i = 0; i < bitVector.size(); i += 8) { 
            std::vector<bool> bits(bitVector.begin() + i, bitVector.begin() + i + 8); 
            decoded += ascii_decode_char(bits); 
        }

        return decoded;
    }

}
