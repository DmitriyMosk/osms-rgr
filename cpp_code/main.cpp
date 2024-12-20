#include <iostream> 
#include <memory>

#include "errors.hpp"
#include "io.hpp"
#include "signal.hpp" 

#define DATA_DIR "./data" 

#define SIGNAL_BUFFER_SIZE 20000
#define SIGNAL_OVERSAMPLING 10

// enable standart modulation
#define SIGNAL_MODULATION_INCLUDE_SIMPLE

int main(int argc, const char *argv[]) {
    std::cout << "------------------" << std::endl;
    // имя-фамилия  
    std::string payload;
    std::cout << "Enter a Firstname Lastname: ";

    if (!std::getline(std::cin, payload)) {
        __err_reading_input_fault(); 
    }

    // signal to noise ratio
    float sigma = 0; 
    std::cout << "Enter a sigma range(0:1): ";

    if (!(std::cin >> sigma)) {
        __err_reading_input_fault(); 
    }

    if (sigma > 1 || sigma < 0) {
        __err_arg_incorrect_sigma();
    }

    // frame struct 
    // |---------PREAMBULA----------|-------PAYLOAD-------|-------CRC-------|
    // -------------31---------------
    
    int carrierFreq = 0; 
    // std::cout << "Enter a Carrier Freq (Hz): "; 

    // if (!(std::cin >> carrierFreq)) { 
    //     __err_reading_input_fault(); 
    // }

    std::cout << "------------------" << std::endl;

    std::shared_ptr<Signal> sig = std::make_shared<Signal>(SIGNAL_BUFFER_SIZE);

    sig->txFreq         = carrierFreq; 
    sig->rxFreq         = carrierFreq;
    sig->txPower        = 1;
    sig->noiseGain      = 1;
    sig->rxThreshold    = 4; 

    sig->ModulationType(SIGNAL_MODULATION_SIMPLE);
    sig->SetBitWide(SIGNAL_OVERSAMPLING);
    // Info

    sig->printConfig();

    // sim

    sig->NoiseDistribution(sigma);

    // process data

    size_t size = sig->InBuffer(
        io::ascii_encode_string(payload)
    ); 

    std::cout << "Incomming buffer data size: " << size << std::endl; 
    
    // transmit
    Signal::TransmitData(sig);
    
    // receive
    Signal::ReceiveData(sig, 0.5);

    // sim save
    Signal::SaveSimulation(sig, DATA_DIR);

    return EXIT_SUCCESS; 
}