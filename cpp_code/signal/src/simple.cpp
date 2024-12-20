#include "signal.hpp"
#include <cmath>

#ifndef SIGNAL_CORRELATION_REFERENCE_THRES
    #define SIGNAL_CORRELATION_REFERENCE_THRES 0.9
#endif

#ifndef SIGNAL_CORRELATION_THRES
    #define SIGNAL_CORRELATION_THRES 0.99
#endif

#ifndef SIGNAL_MAX_PACKET_SIZE
    #define SIGNAL_MAX_PACKET_SIZE 5000
#endif 

std::vector<bool> repeat_elements(const std::vector<bool>& input, int repeat_count) {
    std::vector<bool> result;
    result.reserve(input.size() * repeat_count);  // Резервируем память для результата

    for (const bool& element : input) {
        for (int i = 0; i < repeat_count; ++i) {
            result.push_back(element);
        }
    }

    return result;
}

long double normalized_correlation(const std::vector<double>& A, const std::vector<double>& B) { 
    if (A.size() != B.size()) { 
        throw std::invalid_argument("The size of the vectors (A, B) must be equal.");
    }

    size_t n = A.size();

    double mean_A = std::accumulate(A.begin(), A.end(), 0.0) / n;
    double mean_B = std::accumulate(B.begin(), B.end(), 0.0) / n;

    long double covariance = 0.0;
    long double variance_A = 0.0;
    long double variance_B = 0.0;

    for (size_t i = 0; i < n; ++i) {
        double diff_A = A[i] - mean_A;
        double diff_B = B[i] - mean_B;

        covariance += diff_A * diff_B;
        variance_A += diff_A * diff_A;
        variance_B += diff_B * diff_B;
    }

    // if ((variance_A == 0.0 || variance_B == 0.0) && covariance != 0) {
    //     throw std::runtime_error("Variance of one or both vectors is zero, correlation is undefined.");
    // }
    
    if (variance_A == 0.0 || variance_B == 0.0) {
        return 0;
    }

    return covariance / std::sqrt(variance_A * variance_B);
}


namespace simple { 
    bool window_slide_correlation(std::shared_ptr<Signal> signal, const std::vector<double>& gold_seq, double threshold) {
        std::vector<double>* buff = signal->FGetBuffer();

        if (buff->size() >= gold_seq.size()) {
            std::vector<double> recent_samples(buff->end() - gold_seq.size(), buff->end());

            long double corr = normalized_correlation(recent_samples, gold_seq);

            signal->sim_autoCorrFunc.push_back(corr);

            if (corr > threshold) {
                return true;
            }
        }

        return false;
    }

    void modulate(std::shared_ptr<Signal> signal, uint bitLevel, size_t tau) { 
        std::vector<double> *buff = signal->GetSamples(); 

        size_t samplesCount = buff->size(); 

        for ( size_t i = 0; i < samplesCount; ++i ) 
        { 
            if (buff->at(i) > 0.5) { 
                signal->radioChannelNoise[i + tau] = signal->radioChannelNoise[i + tau] + buff->at(i) * signal->txPower;
            } else { 
                signal->radioChannelNoise[i + tau] = signal->radioChannelNoise[i + tau] - buff->at(i) * signal->txPower;
            }
        }
    }

    void demodulate(std::shared_ptr<Signal> signal, uint bitLevel, double threshold) { 
        std::vector<double> *buff = signal->FGetBuffer(); 

        size_t bit_len = signal->oversampleMult; 

        const uint8_t regXGold = 0b01011; 
        const uint8_t regYGold = 0b11011; 
    
        std::vector<bool> gold_seq = repeat_elements(io::seq::gold_generate(regXGold, regYGold), bit_len);  

        double average_bit_len = 0; 
        double max_thres = 0; 
        double min_thres = 0; 

        size_t corrIndex = 0;

        for (size_t scan = 0; scan < SIGNAL_CHANNEL_LENGTH; ++scan) {
            buff->push_back(signal->radioChannelNoise[scan]);
            
            if (window_slide_correlation(signal, io::seq::convert_gold_seq(gold_seq), SIGNAL_CORRELATION_THRES)) { // search correlation
                corrIndex = scan; 
                break; 
            }
        }
        
        corrIndex += 101; // обрезаем div 

        buff->clear(); 

        std::vector<bool> decoded_bits;

        size_t step = 10; // Каждые 10 семплов
        size_t numSamples = SIGNAL_CHANNEL_LENGTH;

        for (size_t i = corrIndex; i < numSamples; i += step) { 
            double sum = 0.0;

            for (size_t j = 0; j < step && i + j < numSamples; ++j) {
                sum += signal->radioChannelNoise[i + j];
            }

            double average = sum / step;

            // Динамическое пороговое значение с учетом sigma
            double dynamic_threshold = 0.5 + 2 * signal->sigma;

            if (average > dynamic_threshold) { 
                decoded_bits.push_back(1);
            } else { 
                decoded_bits.push_back(0);
            }

            if (decoded_bits.size() >= 2) {
                if (io::crc8_calc(decoded_bits) == 0) {
                   // std::cout << "CRC Passed!" << std::endl;
                    break;
                }
            }
        }

        size_t elementsToCopy = decoded_bits.size() > 17 ? decoded_bits.size() - 17 : 0; 

        if (elementsToCopy > 0) { 
            buff->insert(buff->end(), decoded_bits.begin(), decoded_bits.begin() + elementsToCopy); 
        }

        // std::string receivedString = io::ascii_decode_string(decoded_bits);
        // std::cout << receivedString << std::endl; 

        std::cout << corrIndex << std::endl; 
    }
}

namespace simple_ref { 
    bool window_slide_correlation(std::shared_ptr<Signal> signal, const std::vector<double>& gold_seq, double threshold) {
        std::vector<double>* buff = signal->FGetBufferRef();

        if (buff->size() >= gold_seq.size()) {
            std::vector<double> recent_samples(buff->end() - gold_seq.size(), buff->end());

            long double corr = normalized_correlation(recent_samples, gold_seq);

            signal->sim_autoCorrFuncRef.push_back(corr);

            if (corr > threshold) {
                return true;
            }
        }

        return false;
    }

    void modulate(std::shared_ptr<Signal> signal, uint bitLevel, size_t tau) { 
        std::vector<double> *buff = signal->GetSamples(); 

        // cost
        std::vector<bool> buffBit; 

        for (double val : *buff) { 
            if (val == 1.0) { 
                buffBit.push_back(1); 
            } 

            if (val == 0.0) { 
                buffBit.push_back(0);
            }
        }

        std::vector<double> analogSignal = io::analog::BitsToSignal(buffBit, 2, 1);
        
        for ( size_t i = 0; i < analogSignal.size(); ++i ) 
        {   
            if (analogSignal.at(i) > 0) { 
                signal->radioChannelReference[i + tau] += 1; 
            } else { 
                signal->radioChannelReference[i + tau] += -1; 
            }
        }
    }

    // 10101011101010111000

    void demodulate(std::shared_ptr<Signal> signal, int bitLevel, double threshold) { 
        size_t end_signal = 0; 
        std::vector<bool> div = {1,1,1,1,1,1,1,1,1,1};

        std::vector<double> *buff = signal->FGetBufferRef(); 

        size_t bit_len = signal->oversampleMult; 

        const uint8_t regXGold = 0b01011; 
        const uint8_t regYGold = 0b11011; 
    
        std::vector<bool> gold_seq = repeat_elements(io::seq::gold_generate(regXGold, regYGold), bit_len); 
        std::vector<double> fgold_seq = io::analog::BitsToSignal(gold_seq, 2, 1); 

        size_t corrIndex = 0; 

        for (size_t scan = 0; scan < SIGNAL_CHANNEL_LENGTH; ++scan) { 
            buff->push_back(signal->radioChannelReference[scan]);
            
            if (window_slide_correlation(signal, fgold_seq, SIGNAL_CORRELATION_REFERENCE_THRES)) { // search correlation
                corrIndex = scan; 
                break; 
            }
        }

        buff->clear(); 

        std::cout << "Correlation index: " << corrIndex << std::endl;

        std::vector<double> tmpBuff;
        
        for (size_t conv = corrIndex; conv < SIGNAL_MAX_PACKET_SIZE * signal->oversampleMult ; conv++) { 
            tmpBuff.push_back(signal->radioChannelReference[conv]);
        }

        std::vector<bool> comput = io::analog::SignalToBits(tmpBuff, signal->oversampleMult, bitLevel, 1, 5); 

        for (size_t i = 0; i < comput.size(); i++) { 
            if (comput.at(i)) { 
                end_signal++; 
            } else { 
                end_signal = 0;
            }

            if (end_signal == div.size()) { 
                end_signal = i; 
                break;
            }
        }

        // 10101011101010111000
        // 10101011101010111000

        comput.erase(comput.begin() + end_signal + 1, comput.end());

        std::cout << "xd" << comput.size() << std::endl; 
        for(bool bit : comput) { 
            std::cout << bit; 
        }
        std::cout << std::endl;

        for (size_t i = 0; i < comput.size(); i++) { 

        }

        // if (tmpBuff.size() > 7) { 
        //     size_t elementsToCopy = tmpBuff.size() - 7; 

        //     buff->insert(buff->end(), tmpBuff.begin(), tmpBuff.begin() + elementsToCopy); 
        // }

        // std::string receivedString = io::ascii_decode_string(*buff);
        // std::cout << "Received data from Ref: " << receivedString << std::endl; 
    } 
}