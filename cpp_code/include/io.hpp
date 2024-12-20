#include <cstdint>
#include <vector> 
#include <string> 
#include <algorithm>
#include <iostream>
#include <bitset>

#ifndef IO_HPP 
#define IO_HPP

namespace io { 
    namespace analog { 
        std::vector<double> BitsToSignal(std::vector<bool> bits, int bitLevel, double maxPower);
        int BitsToVal(std::vector<bool> bits); 
        std::vector<bool> SignalToBits(std::vector<double> samples, int samplesPerBit, int bitLevel, double maxPower, double P = 0.1);
    }

    // Head: char to ascii
    // Desc: Возвращает std::vector<bool> (список) битов символа
    std::vector<bool> ascii_encode_char(char c); 

    // Head: ascii to char
    // Desc: Возвращает char (символ) по списку бит из std::vector<bool>
    char ascii_decode_char(const std::vector<bool>& bits); 

    // Head: string to ascii
    // Desc: Возвращает std::vector<bool> (список) битов строки
    std::vector<bool> ascii_encode_string(const std::string &input);

    // Head: ascii to string
    // Desc: Возвращает string (строку) по списку бит из std::vector<bool>
    std::string ascii_decode_string(const std::vector<bool> &bitVector);

    namespace seq { 
        std::vector<double> convert_gold_seq(const std::vector<bool>& gold_seq);

        // Head: Корреляция (недостаточно точно, нужно больше точности)
        // Desc: измеряет корреляцию между A и B последовательностью 
        // Docs: https://ru.wikipedia.org/wiki/%D0%9A%D0%BE%D1%80%D1%80%D0%B5%D0%BB%D1%8F%D1%86%D0%B8%D1%8F
        long double corr(const std::vector<bool> &A, const std::vector<bool> &B);
        
        // Head: Нормализованная корреляция (недостаточно точно, нужно больше точности)
        // Desc: измеряет корреляцию в нормированном виде между A и B последовательностью 
        // Docs: https://ru.wikipedia.org/wiki/%D0%9A%D0%BE%D1%80%D1%80%D0%B5%D0%BB%D1%8F%D1%86%D0%B8%D1%8F
        long double ncorr(const std::vector<bool> &A, const std::vector<bool> &B);

        // Head: Нормализованная побитовая корреляция (недостаточно точно, нужно больше точности)
        // Desc: измеряет корреляцию в нормированном виде между A и B битовой последовательностью 
        // Docs: 4-ая лаба ОСМС be like
        long double nbitcorr(const std::vector<bool> &A, const std::vector<bool> &B); 
        
        // Head: gold sequence
        // Desc: Генерирует 5-битную последовательность голда
        // Docs: https://cyberleninka.ru/article/n/generatsiya-ansambley-kodov-golda-dlya-sistem-pryamogo-rasshireniya-spektra/viewer
        std::vector<bool> gold_generate(uint8_t regX, uint8_t regY);

        // Head: kasami sequence
        // Desc: Генерирует 5-битную последовательность Касами
        // Docs: https://cyberleninka.ru/article/n/generatsiya-ansambley-kodov-golda-dlya-sistem-pryamogo-rasshireniya-spektra/viewer
        std::vector<bool> kasami_generate(uint8_t regX, uint8_t regY);

        // Head: Barker sequence
        // Desc: Генерирует 5-битную последовательность Баркера
        // Docs: https://cyberleninka.ru/article/n/generatsiya-ansambley-kodov-golda-dlya-sistem-pryamogo-rasshireniya-spektra/viewer
        std::vector<bool> barker_generate(uint8_t regX, uint8_t regY, uint length); 

        namespace validate { 
            bool prs_is_balanced(const std::vector<bool> &seq);
            bool prs_is_cycled(const std::vector<bool> &seq);
            bool prs_is_correlated(const std::vector<bool> &seq);
        }
    }  

    namespace file { 
           
    }

    // Head: crc8 calc
    // Desc: Возвращает 8-ми битный crc (7 бит по 8бит полиному)
    uint8_t crc8_calc(const std::vector<bool>& input);

    // Head: crc8 check
    // Desc: Проверяет целостность данных по crc8
    bool crc8_check(const std::vector<bool>& inputWithCrc);
} 

#endif 