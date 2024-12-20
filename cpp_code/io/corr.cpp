#include "io.hpp" 

/*
N - [0b00000; 0b11111] # 5 bits

XOR_MASK - 0b00011 (first) or 0b01001 (second) 

N & XOR_MASK_1 = [0b00000 or 0b00001 or 0b00010 or 0b00011]; 
N & XOR_MASK_2 = [0b00000 or 0b00001 or 0b01000 or 0b01001];  

XOR wait (XOR_MASK_1): 
0b00000 XOR 1 and 2 bit = 0 (0b00000 ^ XOR_MASK_1)  = XOR_MASK_1
0b00001 XOR 1 and 2 bit = 1 (0b00001 ^ XOR_MASK_1) != XOR_MASK_1
0b00010 XOR 1 and 2 bit = 1 (0b00010 ^ XOR_MASK_1) != XOR_MASK_1
0b00011 XOR 1 and 2 bit = 0 (0b00011 ^ XOR_MASK_1) != XOR_MASK_1 but eq 0b00000

XOR wait (XOR_MASK_2): 
0b00000 XOR 1 and 4 bit = 0 (0b00000 ^ XOR_MASK_2)  = XOR_MASK_2
0b00001 XOR 1 and 4 bit = 1 (0b00001 ^ XOR_MASK_2) != XOR_MASK_2
0b01000 XOR 1 and 4 bit = 1 (0b01000 ^ XOR_MASK_2) != XOR_MASK_2
0b01001 XOR 1 and 4 bit = 0 (0b01001 ^ XOR_MASK_2) != XOR_MASK_2 but eq 0b00000
*/

namespace io { 
    namespace seq { 
        std::vector<double> convert_gold_seq(const std::vector<bool>& gold_seq) {
            std::vector<double> result;
            result.reserve(gold_seq.size());
            for (bool bit : gold_seq) {
                result.push_back(bit ? 1.0 : -1.0);
            }
            return result;
        }   

        long double nbitcorr(const std::vector<bool> &A, const std::vector<bool> &B) { 
            if (A.size() != B.size()) { 
                throw std::invalid_argument("The size of the bit vectors (A, B) must be equal.");
            }

            int count_mismatches = 0; 
            int count_matchces   = 0; 

            for (size_t i = 0; i < A.size() && i < B.size(); ++i) { 
                if (A[i] != B[i]) { 
                    ++count_mismatches; 
                } else { 
                    ++count_matchces;
                }
            }

            return (count_matchces - 2 * count_mismatches) / static_cast<long double>(A.size()); 
        }

        long double cnbitcorr(const std::vector<bool> &A, uint lag) { 
            std::vector<bool> tmpA = A; 
            
            // right shift
            std::rotate(tmpA.rbegin(), tmpA.rbegin() + lag, tmpA.rend());

            return nbitcorr(A, tmpA); 
        } 

        std::vector<long double> anbitcorr(const std::vector<bool> &A) { 
            std::vector<long double> autoCorr(A.size() + 1); 

            for (size_t i = 0; i <= A.size(); i++) { 
                autoCorr[i] = cnbitcorr(A, i); 
            }

            return autoCorr; 
        }

        std::vector<bool> gold_generate(uint8_t regX, uint8_t regY) { 
            if (regX > 0x1F || regY > 0x1F) { 
                return {0,0}; 
            }

            // 2^n - 1; n = 5
            const uint length = 31;

            // polynom sequence L 
            const uint8_t scheme_l = 0b00011;

            // polynom sequence M 
            const uint8_t scheme_m = 0b01001; 

            // polynom sequence gold 
            const uint8_t scheme_g = 0b00001;
            
            std::vector<bool> gold(length); 

            for(uint i = 0; i < length; ++i) { 
                uint8_t reg_x_xor = (regX & scheme_l) ^ scheme_l; 
                uint8_t reg_y_xor = (regX & scheme_m) ^ scheme_m; 

                gold[i] = static_cast<bool>((regX & scheme_g) ^ (regY & scheme_g)); 

                regX >>= 1;
                regY >>= 1;
                
                regX |= (reg_x_xor == scheme_l || reg_x_xor == 0b00000 ) ? 0b00000 : 0b10000;
                regY |= (reg_y_xor == scheme_m || reg_y_xor == 0b00000 ) ? 0b00000 : 0b10000;
            }

            return gold;
        }

        // Kasami sequence
        std::vector<bool> kasami_generate(uint8_t regX, uint8_t regY) { 
            return {0,0};
        }
         
        // Barker sequence 
        std::vector<bool> barker_generate(uint8_t regX, uint8_t regY, uint length) { 
            return {0,0};
        } 

        namespace validate { 
            bool prs_is_balanced(const std::vector<bool>& seq) { 
                int count_zeros     = std::count(seq.begin(), seq.end(), 0); 
                int count_ones      = std::count(seq.begin(), seq.end(), 1);

                if (count_ones + count_zeros != 31) { 
                    return false;
                }

                if (abs(count_zeros - count_ones) != 1) {
                    return false;
                }

                return true;
            }   

            bool prs_is_cycled(const std::vector<bool>& seq) { 
                bool valid = true; 

                std::vector<uint> cycle_lengths;

                uint cycle_length = 1;

                for (size_t i = 1; i < seq.size(); ++i) {
                    if (seq[i] == seq[i - 1]) {
                        ++cycle_length;
                    } else {
                        cycle_lengths.push_back(cycle_length);
                        cycle_length = 1;
                    }
                }

                std::sort(cycle_lengths.begin(), cycle_lengths.end());

                if (cycle_lengths.back() > 4) {
                    valid = false;
                }

                uint cycle_count = cycle_lengths.size(); 

                uint lastCycle = 0; 

                for (uint i = 1; i < cycle_lengths.back(); ++i) { 
                    uint subcycles = std::count(cycle_lengths.begin(), cycle_lengths.end(), cycle_lengths[i]); 

                    if (lastCycle == 0) { 
                        lastCycle = subcycles; 
                        if (cycle_count / lastCycle != 2) { 
                            valid = false; 
                        }
                    } else { 
                        if (subcycles % lastCycle == 0) { 
                            lastCycle = subcycles;
                        } else { 
                            valid = false; 
                            break; 
                        }
                    }
                }

                return valid;
            }

            bool prs_is_correlated(const std::vector<bool>& seq) {
                std::vector<long double> autoCorrF = anbitcorr(seq); 

                long double min_elem(0.0);
                uint        min_count(0);
                auto        min_elem_it = std::min_element(autoCorrF.begin(), autoCorrF.end());


                if (min_elem_it != autoCorrF.end()) {
                    min_elem = *min_elem_it;
                } else {
                    throw std::runtime_error("Min element undefined.");
                }

                for (long double aF : autoCorrF) { 
                    if ( aF <= min_elem ) { 
                        ++min_count;
                    }
                }

                if ( static_cast<double>(autoCorrF.size()) / static_cast<double>(min_count) > 2 ) { 
                    return false; 
                }

                return true; 
            }
        }
    }
}

 