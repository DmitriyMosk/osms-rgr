#include "signal.hpp"

void Signal::ReceiveData(std::shared_ptr<Signal> signal, double threshold) { 
    if (signal->modType == SIGNAL_MODULATION_AM) { 
        std::vector<double> noiseDemodulation = am::demodulate(signal->txFreq, &signal->radioChannelNoise, 1);
        std::vector<double> referDemodulation = am::demodulate(signal->txFreq, &signal->radioChannelReference, 1);
    }

    if (signal->modType == SIGNAL_MODULATION_QPSK) { 
        
    }

    if (signal->modType == SIGNAL_MODULATION_OFDM) { 

    }

    if (signal->modType == SIGNAL_MODULATION_SIMPLE) { 
        simple_ref::demodulate(signal, 2, threshold);

        signal->sim_ioBuffer_ref.insert(signal->sim_ioBuffer_ref.end(), signal->ioBuffer_ref.begin(), signal->ioBuffer_ref.end()); 

        simple::demodulate(signal, 2, threshold); 

        signal->sim_ioBuffer.insert(signal->sim_ioBuffer.end(), signal->ioBuffer.begin(), signal->ioBuffer.end());
    }
}

/*
    2-х 4-ёх и 8-ми уровневая кодировка
    устойчивость к помехам

    bitErrorRate график от sigma для трёх
*/