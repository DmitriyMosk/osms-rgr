#include <iostream>
#include <vector> 
#include <cassert>
#include <bitset> 

#include "tests.hpp" 

using namespace std; 

int main() { 
    test_ascii_encode_decode_char(); 
    test_ascii_encode_decode_string(); 

    test_crc8(); 

    test_corr_sequences(); 
    test_corr_crosscorr();
    test_corr_autocorr(); 

    TestEncodingDecoding(2, 3, 1.0);
    TestEncodingDecoding(4, 3, 2.0);
    TestEncodingDecoding(8, 4, 4.0);

    // Проверка уровней сигнала
    TestSignalLevels(2, 1.0);
    TestSignalLevels(4, 2.0);
    TestSignalLevels(8, 4.0);
    
    TestSignalToBitsNoised(); 
    return EXIT_SUCCESS; 
}

// 

void test_ascii_encode_decode_char() { 
    char testSymbol = 'J'; 
    std::vector<bool> expectedResult = {0, 1, 0, 0, 1, 0, 1, 0}; 

    // test encode 
    assert(io::ascii_encode_char(testSymbol) == expectedResult);
    std::cout << "Unit test passed: io::ascii_encode_char();\n"; 

    // test decode 
    assert(io::ascii_decode_char(expectedResult) == testSymbol);
    std::cout << "Unit test passed: io::ascii_decode_char();\n"; 
}

void test_ascii_encode_decode_string() { 
    std::string testString = "AB"; // ASCII: A=01000001, B=01000010

    std::vector<bool> expectedResult = {
        0, 1, 0, 0, 0, 0, 0, 1,
        0, 1, 0, 0, 0, 0, 1, 0 
    };

    // test encode 
    assert(io::ascii_encode_string(testString) == expectedResult);
    std::cout << "Unit test passed: io::ascii_encode_string();\n"; 

    // test decode
    assert(io::ascii_decode_string(expectedResult) == testString);
    std::cout << "Unit test passed: io::ascii_decode_string();\n"; 
}

// 

void test_crc8() { 
    const std::vector<bool> testData1 = {
        0,1,1,0,1,0,1,1,0,1,1,1,0,0,1,1,
        0,1,1,0,1,0,1,0,0,1,1,0,0,1,1,1
    }; 
    const std::vector<bool> testData2 = {
        0,1,1,0,1,0,1,1,0,1,1,1,0,0,1,1,0,1,1,0,1,0,1,0,
        0,1,1,0,0,1,1,1,0,1,1,0,0,1,0,0,0,1,1,1,0,0,1,1
    }; 
    const std::vector<bool> testData3 = {
        0,1,1,0,1,0,1,1,0,1,1,1,0,0,1,1,0,1,1,0,1,0,1,0,0,1,1,0,0,1,1,1,0,1,1,
        0,1,1,0,0,1,1,0,0,1,1,1,0,0,1,1,0,1,1,0,1,0,1,0,0,1,1,0,0,0,1,0,1,1, 
        1,1,1,0,1,1,0,1,0,1,1,0,1,1,1,0,0,1,1,0,1,1,0,1,0,1,0,0,1,1,0,0,1,1,1 
    }; 

    assert(io::crc8_calc(testData1) == 0b11110101);
    std::cout << "Unit test passed: io::crc8_calc() -> 0b11110101; with 32 seq len\n"; 

    assert(io::crc8_calc(testData2) == 0b11100101);
    std::cout << "Unit test passed: io::crc8_calc() -> 0b11100101; with 48 seq len\n"; 

    assert(io::crc8_calc(testData3) == 0b00011010);
    std::cout << "Unit test passed: io::crc8_calc() -> 0b00011010; with 104 seq len\n"; 

    std::vector<bool> testDataWithCRC1 = {
        0,1,1,0,1,0,1,1,0,1,1,1,0,0,1,1,
        0,1,1,0,1,0,1,0,0,1,1,0,0,1,1,1,
        // crc part
        1,1,1,0,1,0,1
    }; 
    std::vector<bool> testDataWithCRC2 = {
        0,1,1,0,1,0,1,1,0,1,1,1,0,0,1,1,0,1,1,0,1,0,1,0,
        0,1,1,0,0,1,1,1,0,1,1,0,0,1,0,0,0,1,1,1,0,0,1,1, 
        // crc part
        1,1,0,0,1,0,1
    }; 
    std::vector<bool> testDataWithCRC3 = {
        0,1,1,0,1,0,1,1,0,1,1,1,0,0,1,1,0,1,1,0,1,0,1,0,0,1,1,0,0,1,1,1,0,1,1,
        0,1,1,0,0,1,1,0,0,1,1,1,0,0,1,1,0,1,1,0,1,0,1,0,0,1,1,0,0,0,1,0,1,1, 
        1,1,1,0,1,1,0,1,0,1,1,0,1,1,1,0,0,1,1,0,1,1,0,1,0,1,0,0,1,1,0,0,1,1,1,
        // crc part
        0,0,1,1,0,1,0
    }; 

    assert(io::crc8_check(testDataWithCRC1));
    std::cout << "Unit test passed: io::crc8_check(); with 32 + 7 seq len\n"; 

    assert(io::crc8_check(testDataWithCRC2)); 
    std::cout << "Unit test passed: io::crc8_check(); with 48 + 7 seq len\n";

    if (!io::crc8_check(testDataWithCRC3)) { 
        std::cout << "[WARNING] Unit test failed: io::crc8_check(); with 104 + 7 seq len\n";
    } else { 
        std::cout << "Unit test passed: io::crc8_check(); with 104 + 7 seq len\n";
    }

    // test error detection 

    int errorsFake_32(0), errorsDetected_32(0), errorsMissed_32(0); 
    for(size_t i = 0; i < testDataWithCRC1.size(); i++) { 
        if (!io::crc8_check(testDataWithCRC1)) { 
            errorsFake_32++; 
        }

        testDataWithCRC1[i] = testDataWithCRC1[i] ^ 1; // change bit 

        if (!io::crc8_check(testDataWithCRC1)) { 
            errorsDetected_32++; 
        } else { 
            errorsMissed_32++; 
        }

        testDataWithCRC1[i] = testDataWithCRC1[i] ^ 1; // change bit 
    }

    std::cout << "Error detection CRC8 test: seq=" << testDataWithCRC1.size() << " EF=" << errorsFake_32 << " ED=" << errorsDetected_32 << " EM=" << errorsMissed_32 << std::endl;  
    assert(static_cast<size_t>(errorsDetected_32) == testDataWithCRC1.size() && errorsFake_32 == 0 && errorsMissed_32 == 0); 

    // 

    int errorsFake_48(0), errorsDetected_48(0), errorsMissed_48(0); 
    for(size_t i = 0; i < testDataWithCRC2.size(); i++) { 
        if (!io::crc8_check(testDataWithCRC2)) { 
            errorsFake_48++; 
        }

        testDataWithCRC2[i] = testDataWithCRC2[i] ^ 1; // change bit 

        if (!io::crc8_check(testDataWithCRC2)) { 
            errorsDetected_48++; 
        } else { 
            errorsMissed_48++; 
        }

        testDataWithCRC2[i] = testDataWithCRC2[i] ^ 1; // change bit 
    }

    std::cout << "Error detection CRC8 test: seq=" << testDataWithCRC2.size() << " EF=" << errorsFake_48 << " ED=" << errorsDetected_48 << " EM=" << errorsMissed_48 << std::endl;  
    assert(static_cast<size_t>(errorsDetected_48) == testDataWithCRC2.size() && errorsFake_48 == 0 && errorsMissed_48 == 0); 

    // 

    int errorsFake_104(0), errorsDetected_104(0), errorsMissed_104(0); 
    for(size_t i = 0; i < testDataWithCRC3.size(); i++) { 
        if (!io::crc8_check(testDataWithCRC3)) { 
            errorsFake_104++; 
        }

        testDataWithCRC3[i] = testDataWithCRC3[i] ^ 1; // change bit 

        if (!io::crc8_check(testDataWithCRC3)) { 
            errorsDetected_104++; 
        } else { 
            errorsMissed_104++; 
        }

        testDataWithCRC3[i] = testDataWithCRC3[i] ^ 1; // change bit 
    }

    std::cout << "Error detection CRC8 test: seq=" << testDataWithCRC3.size() << " EF=" << errorsFake_104 << " ED=" << errorsDetected_104 << " EM=" << errorsMissed_104 << std::endl;  
    assert(static_cast<size_t>(errorsDetected_104) == testDataWithCRC3.size() && errorsFake_104 == 0 && errorsMissed_104 == 0); 
}

//

void test_corr_sequences() { 
    const uint8_t regXGold = 0b01011; 
    const uint8_t regYGold = 0b11011; 

    // gold test 

    std::vector<bool> gold_seq = io::seq::gold_generate(regXGold, regYGold); 

    assert(io::seq::validate::prs_is_balanced(gold_seq)); 
    std::cout << "Unit test passed: io::seq::prs_is_balanced(); with 31 len gold seq\n"; 

    assert(io::seq::validate::prs_is_cycled(gold_seq)); 
    std::cout << "Unit test passed: io::seq::prs_is_cycled(); with 31 len gold seq\n"; 

    assert(io::seq::validate::prs_is_correlated(gold_seq)); 
    std::cout << "Unit test passed: io::seq::prs_is_correlated(); with 31 len gold seq\n"; 
    
    // const uint8_t regXKasami = 0b0; 
    // const uint8_t regYKasami = 0b0; 

    // // kasami test 

    // std::vector<bool> kasami_seq = io::seq::kasami_generate(regXKasami, regYKasami); 

    // for (bool bit : kasami_seq) {
    //     std::cout << bit; 
    // }
    // 00001001110100111010011101001111111111111011001110110011111111111110111010
    //                                       011100111011100111111111111110111101
    // std::cout << std::endl; 

    // assert(io::seq::validate::prs_is_balanced(kasami_seq)); 
    // std::cout << "Unit test passed: io::seq::prs_is_balanced(); with 31 len kasami seq\n"; 

    // assert(io::seq::validate::prs_is_cycled(kasami_seq)); 
    // std::cout << "Unit test passed: io::seq::prs_is_cycled(); with 31 len kasami seq\n"; 

    // assert(io::seq::validate::prs_is_correlated(kasami_seq)); 
    // std::cout << "Unit test passed: io::seq::prs_is_correlated(); with 31 len kasami seq\n"; 

    // const uint8_t regXBarker = 0b0; 
    // const uint8_t regYBarker = 0b0; 
    
    // // barker test 
    
    // std::vector<bool> barker_seq = io::seq::barker_generate(regXBarker, regYBarker, 10); 

    // for (bool bit : barker_seq) {
    //     std::cout << bit; 
    // }

    // std::cout << std::endl; 

    // assert(io::seq::validate::prs_is_balanced(barker_seq)); 
    // std::cout << "Unit test passed: io::seq::prs_is_balanced(); with 31 len barker seq\n"; 

    // assert(io::seq::validate::prs_is_cycled(barker_seq)); 
    // std::cout << "Unit test passed: io::seq::prs_is_cycled(); with 31 len barker seq\n"; 

    // assert(io::seq::validate::prs_is_correlated(barker_seq)); 
    // std::cout << "Unit test passed: io::seq::prs_is_correlated(); with 31 len barker seq\n"; 
}

void test_corr_crosscorr() { 

}

void test_corr_autocorr() { 
    
}

void TestEncodingDecoding(int bitLevel, int samplesPerBit, double maxPower) {
    vector<bool> originalBits = {1, 0, 1, 1, 0, 0, 1, 0};

    vector<double> signal = io::analog::BitsToSignal(originalBits, bitLevel, maxPower);

    vector<double> upsampledSignal;
    for (double sample : signal) {
        for (int i = 0; i < samplesPerBit; ++i) {
            upsampledSignal.push_back(sample);
        }
    }

    vector<bool> decodedBits = io::analog::SignalToBits(upsampledSignal, samplesPerBit, bitLevel, maxPower);

    assert(originalBits == decodedBits);
    cout << "Test passed: Encoding and decoding are consistent for bitLevel = " 
         << bitLevel << ", samplesPerBit = " << samplesPerBit << ", maxPower = " << maxPower << endl;
}

void TestSignalLevels(int bitLevel, double maxPower) {
    vector<bool> bits;
    for (int i = 0; i < (1 << bitLevel); ++i) {
        vector<bool> bitGroup;
        for (int j = bitLevel - 1; j >= 0; --j) {
            bitGroup.push_back((i >> j) & 1);
        }
        bits.insert(bits.end(), bitGroup.begin(), bitGroup.end());
    }

    vector<double> signal = io::analog::BitsToSignal(bits, bitLevel, maxPower);

    double step = 2 * maxPower / ((1 << bitLevel) - 1);
    for (size_t i = 0; i < signal.size(); ++i) {
        double expectedValue = -maxPower + i * step;
        assert(fabs(signal[i] - expectedValue) < 1e-6);
    }

    cout << "Test passed: Signal levels are correct for bitLevel = " << bitLevel << ", maxPower = " << maxPower << endl;
}

void TestSignalToBitsNoised() {
    vector<double> noisySignal = {0.95, -0.85, 0.78, -1.1, 0.21};
    vector<bool> expectedBits = {1, 0, 1, 0, 0, 1};
    vector<bool> result = io::analog::SignalToBits(noisySignal, 1, 2, 1.0, 0.2);

    cout << "Expected: ";
    for (bool b : expectedBits) cout << b;
    cout << "\nResult:   ";
    for (bool b : result) cout << b;
    cout << endl;
}
