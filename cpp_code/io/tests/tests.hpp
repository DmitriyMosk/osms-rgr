#include "io.hpp" 
#include <cmath>

#ifndef IO_HPP_TESTS
#define IO_HPP_TESTS
    void test_ascii_encode_decode_char(); 
    void test_ascii_encode_decode_string(); 

    void test_corr_sequences(); 
    void test_corr_crosscorr();
    void test_corr_autocorr(); 

    void test_crc8(); 

    void TestEncodingDecoding(int bitLevel, int samplesPerBit, double maxPower);
    void TestSignalLevels(int bitLevel, double maxPower);
    void TestSignalToBitsNoised(); 
#endif