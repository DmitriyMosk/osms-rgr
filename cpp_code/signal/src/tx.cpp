#include "signal.hpp"

// static
void Signal::TransmitData(std::shared_ptr<Signal> signal) { 
    // oversample 
    signal->Oversampling(); 
    signal->GenerateNoise(); 
    
    size_t tau = 5000; 

    if (signal->modType == SIGNAL_MODULATION_AM) { 
        //std::vector<double> out = am::modulate(signal, 1);
    }

    if (signal->modType == SIGNAL_MODULATION_QPSK) { 
        
    }

    if (signal->modType == SIGNAL_MODULATION_OFDM) { 

    }

    if (signal->modType == SIGNAL_MODULATION_SIMPLE) { 
        simple_ref::modulate(signal, 2, tau); 
        //
        simple::modulate(signal, 2, tau); 
    }

    signal->emit(false); 
}