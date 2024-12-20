#include <vector>
#include <functional>
#include <iostream> 
#include <cstdint>
#include <random>
#include <fstream>
#include <optional>
#include <memory>
#include <format>
#include <string>
#include <iterator>

#include "errors.hpp"
#include "io.hpp"

#ifndef SIGNAL_HPP 
#define SIGNAL_HPP 

// Определяет размер имитируемого канала
#define SIGNAL_CHANNEL_LENGTH 80000 // 80k
/*
    Fields: 
    //buffers&pseudoBuffers

    Используется для имитации зашумлённого канала связи.
    std::vector<double> radioChannelNoise; 

    Используется для имитации эталонного канала связи.
    std::vector<double> radioChannelReference; 

    Используется для имитации буфера устройства RxTx
    size_t bufferSize;
    size_t oversampleMult;
    std::vector<bool> ioBuffer;

    std::string modType;
    
    float sigma;

    public:
        Signal(int bufferSize);

        // conf Methods
        int sampleRate;
        int txFreq, rxFreq;
        int txPower, noisePower;
*/
class Signal {
private:
    //buffers&pseudoBuffers
    /* 
        Используется для имитации буфера устройства RxTx
    */
    size_t bufferSize;
    std::vector<bool> ioBuffer;
    std::vector<bool> ioBuffer_ref; 
    
    std::vector<double> ioFBuffer; 
    std::vector<double> ioFBuffer_ref;
    /* 
        Используется для имитации буфера (модулятора Tx)
    */
    std::vector<double> modulationBuffer; 

    /* 
        Используется для имитации буфера (модулятора RxTx)
    */
    std::vector<double> samplesBuffer; 

    std::string modType;
    
public:
    float sigma;
    //buffers&pseudoBuffers
    /*
        Используется для имитации зашумлённого канала связи.
    */
    std::vector<double> radioChannelNoise; 

    /*
        Используется для имитации эталонного канала связи.
    */
    std::vector<double> radioChannelReference; 

    std::vector<bool>   sim_ioBuffer; 
    std::vector<bool>   sim_ioBuffer_ref; 
    std::vector<double> sim_ioFBuffer;
    std::vector<double> sim_modulationBuffer; 
    std::vector<double> sim_samplesBuffer; 
    std::vector<double> sim_autoCorrFunc; 
    std::vector<double> sim_autoCorrFuncRef; 

    Signal(int bufferSize);

    // conf
    int txFreq, rxFreq;
    int txPower, noiseGain;
    int rxThreshold;
    size_t oversampleMult;

    void NoiseDistribution(float val);
    void ModulationType(std::string t);
    void SetBitWide(size_t oversample);
    void Oversampling(); 
    void GenerateNoise();

    // buffer operations
    size_t InBuffer(const std::vector<bool> bits);
    std::optional<bool> OutBuffer();
    std::vector<bool> *GetBuffer();

    std::vector<double> *FGetBuffer();      // noised channel
    std::vector<double> *FGetBufferRef();   // reference channel

    std::vector<bool> *GetBufferRef();
    std::vector<double> *GetSamples(); 
    std::vector<double> *ModulationBuffer();

    #define SIGNAL_BUFFER_IS_EMPTY      -1
    #define SIGNAL_BUFFER_FLUSH         0
    #define SIGNAL_BUFFER_FLUSH_FAULT   1  

    int FlushBuffer(); 

    void printConfig();
    void emit(bool deleteSample);

    // public static

    static void TransmitData(std::shared_ptr<Signal> sig);
    static void ReceiveData(std::shared_ptr<Signal> signal, double threshold);

    static void SaveSimulation(std::shared_ptr<Signal> sig, const std::string path);

    static std::vector<bool> Framing(std::vector<bool> data);
};

#ifndef SIGNAL_MODULATION_INCLUDE_QPSK

#define SIGNAL_MODULATION_QPSK "qpsk" 
namespace qpsk 
{
    std::vector<double> modulate(uint64_t carrierFreq, std::vector<bool> bits); 

    std::vector<double> demodulate(uint64_t carrierFreq); 
}

#endif // SIGNAL_MODULATION_INCLUDE_QPSK

#ifndef SIGNAL_MODULATION_INCLUDE_OFDM 

#define SIGNAL_MODULATION_OFDM "ofdm" 
namespace ofdm 
{
    std::vector<double> modulate(uint64_t carrierFreq, std::vector<bool> bits); 

    std::vector<double> demodulate(uint64_t carrierFreq); 
}

#endif // SIGNAL_MODULATION_INCLUDE_OFDM

#ifndef SIGNAL_MODULATION_INCLUDE_AM 

#define SIGNAL_MODULATION_AM "am" 
namespace am 
{
    std::vector<double> modulate(std::shared_ptr<Signal> signal, uint bitLevel);

    std::vector<double> demodulate(uint64_t carrierFreq, std::vector<double> *channel, uint bitLevel); 
}

#endif // SIGNAL_MODULATION_INCLUDE_AM

#ifndef SIGNAL_MODULATION_INCLUDE_SIMPLE

#define SIGNAL_MODULATION_SIMPLE "simple" 
namespace simple 
{
    bool window_slide_correlation(std::shared_ptr<Signal> signal, std::vector<bool> gold_seq, double threshold);
    void modulate(std::shared_ptr<Signal> signal, uint bitLevel, size_t tau); 
    void demodulate(std::shared_ptr<Signal> signal, uint bitLevel, double threshold);
}

namespace simple_ref {
    bool window_slide_correlation(std::shared_ptr<Signal> signal, const std::vector<double>& gold_seq, double threshold);
    void modulate(std::shared_ptr<Signal> signal, uint bitLevel, size_t tau); 
    void demodulate(std::shared_ptr<Signal> signal, int bitLevel, double threshold);
}

#endif // SIGNAL_MODULATION_INCLUDE_SIMPLE

#endif // SIGNAL_HPP
