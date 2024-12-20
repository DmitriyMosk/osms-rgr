#include "signal.hpp"

namespace iframe { 
    std::vector<bool> div = {1,1,1,1,1,1,1,1,1,1};

    void insert_gold(std::vector<bool> &frame) { 
        const uint8_t regXGold = 0b01011; 
        const uint8_t regYGold = 0b11011; 
    
        std::vector<bool> gold_seq = io::seq::gold_generate(regXGold, regYGold);    

        frame.insert(frame.end(), gold_seq.begin(), gold_seq.end()); 
    }

    void insert_div(std::vector<bool> &frame) {  
        frame.insert(frame.end(), div.begin(), div.end()); 
    }

    void insert_payload(std::vector<bool> &frame, std::vector<bool> payload) { 
        frame.insert(frame.end(), payload.begin(), payload.end()); 
    }

    void insert_crc(std::vector<bool> &frame) { 
        uint8_t crc8 = io::crc8_calc(frame); 

        for (int i = 6; i >= 0; --i) { 
            bool bit = (crc8 >> i) & 1; 
            frame.push_back(bit); 
        }
    }   

    void insert_hemming(std::vector<bool> frame) { 

    }
}

Signal::Signal(int bufferSize = 0) {
    if (bufferSize <= 0) { 
        __err_arg_incorrect_io_buffer_size(); 
        return;
    }

    radioChannelReference.resize(SIGNAL_CHANNEL_LENGTH); 
    radioChannelNoise.resize(SIGNAL_CHANNEL_LENGTH); 
    
    this->bufferSize = bufferSize; 

    //default
    this->oversampleMult    = 1;
    this->rxFreq            = 0; 
    this->txFreq            = 0; 
    this->txPower           = 1; 
    this->noiseGain         = 1;
    this->modType           = "";
}

void Signal::ModulationType(std::string t) { 
    if (t == "am" || t == "qpsk" || t == "simple") { 
        modType = t;
    } else { 
        __err_unknown_modulation_type();
    }
}

std::vector<double> *Signal::ModulationBuffer() { 
    return &this->modulationBuffer;
}

void Signal::Oversampling() { 
    for (bool bit : this->ioBuffer) { 
        samplesBuffer.insert(samplesBuffer.end(), this->oversampleMult, static_cast<double>(bit));  
    }

    sim_ioBuffer.insert(sim_ioBuffer.end(), ioBuffer.begin(), ioBuffer.end()); 
    sim_samplesBuffer.insert(sim_samplesBuffer.end(), samplesBuffer.begin(), samplesBuffer.end()); 

    if (this->FlushBuffer() != SIGNAL_BUFFER_FLUSH_FAULT) { 
        std::cerr << "Flush buffer fault" << std::endl; 
    }
}

std::vector<double> *Signal::GetSamples() { 
    return &this->samplesBuffer;
}

std::vector<bool> Signal::Framing(std::vector<bool> data) { 
    auto corrSeq = std::make_unique<std::vector<bool>>(); 

    iframe::insert_gold(*corrSeq); 

    auto frame = std::make_unique<std::vector<bool>>(); 
    
    iframe::insert_payload(*frame, data); 
    // iframe::insert_hemming(*frame); 
    // iframe::insert_div(*frame);
    iframe::insert_crc(*frame); 
    iframe::insert_div(*frame); 

    auto combined = std::make_unique<std::vector<bool>>();

    combined->insert(combined->end(), corrSeq->begin(), corrSeq->end()); 
    combined->insert(combined->end(), frame->begin(), frame->end());

    /*
        some processing  
    */

    return *combined;
}

size_t Signal::InBuffer(const std::vector<bool> bits) { 
    std::vector<bool> frame = this->Framing(bits); 

    for (bool bit : frame) { 
        if (ioBuffer.size() == this->bufferSize) {
            __err_buffer_overflow(); 
            return -1;
        }

        ioBuffer.push_back(bit);  
    }

    return ioBuffer.size();
}

std::vector<bool> *Signal::GetBuffer() { 
    return &this->ioBuffer;
}

std::vector<double> *Signal::FGetBuffer() { 
    return &this->ioFBuffer;
}

std::vector<double> *Signal::FGetBufferRef() { 
    return &this->ioFBuffer_ref;
}

std::vector<bool> *Signal::GetBufferRef() { 
    return &this->ioBuffer_ref;
}

int Signal::FlushBuffer() { 
    if (ioBuffer.size() == 0) { 
        return SIGNAL_BUFFER_IS_EMPTY;
    }

    ioBuffer.clear();

    return SIGNAL_BUFFER_FLUSH;
}

void Signal::NoiseDistribution(float val) { 
    if (val < 0.0 || val > 1.0) { 
        __err_arg_incorrect_sigma();
    }

    sigma = val;
}

void Signal::GenerateNoise() { 
    std::mt19937 generator(std::random_device{}());

    std::normal_distribution<double> distribution(0.0, sigma);

    for (int i = 0; i < SIGNAL_CHANNEL_LENGTH; ++i) {
        double noise = distribution(generator) * this->noiseGain;
        radioChannelNoise[i] = noise;
    }

    for (int i = 0; i < SIGNAL_CHANNEL_LENGTH; ++i) { 
        radioChannelReference[i] = 0;
    }   
    
    auto min_max = std::minmax_element(radioChannelNoise.begin(), radioChannelNoise.end()); 

    double min_val = *min_max.first; 
    double max_val = *min_max.second; 

    for (int i = 0; i < SIGNAL_CHANNEL_LENGTH; ++i) { 
        radioChannelNoise[i] = (radioChannelNoise[i] - min_val) / (max_val - min_val); 
    }
} 

void Signal::SaveSimulation(std::shared_ptr<Signal> sig, const std::string path) { 
    std::string simData  = path + "/simdata.txt"; 
    std::string simFrame = path + "/buffer.txt";
    std::string simSamples = std::format("{}/buffer_{}.txt", path, sig->oversampleMult);

    std::string simChannelNoise = std::format("{}/ch_nouse_{}.txt", path, sig->sigma);
    std::string simChannelReference = std::format("{}/ch_reference_{}.txt", path, sig->sigma);

    std::string simBufferDemRef = path + "/buffer_dem_ref.txt";
    std::string simBufferDem = std::format("{}/buffer_dem{}.txt", path, sig->sigma);

    std::string simAutoCorr = std::format("{}/ch_autocorr_{}.txt", path, sig->sigma);
    std::string simAutoCorrRef = std::format("{}/ch_autocorr.txt", path);

    std::ofstream fileSim(simData), 
        fileFrame(simFrame), 
        fileSamples(simSamples), 
        fileChannelNoise(simChannelNoise), 
        fileChannelReference(simChannelReference), 
        fileAutoCorrFunc(simAutoCorr),
        fileBufferDemRef(simBufferDemRef), 
        fileBufferDem(simBufferDem),
        fileAutoCorrRef(simAutoCorrRef);

    if (!fileSim.is_open()                  || 
        !fileFrame.is_open()                || 
        !fileSamples.is_open()              || 
        !fileChannelNoise.is_open()         || 
        !fileChannelReference.is_open()     || 
        !fileAutoCorrFunc.is_open()         || 
        !fileBufferDemRef.is_open()         || 
        !fileBufferDem.is_open()            || 
        !fileAutoCorrRef.is_open()) { 
        __err_fstream_fault(); 
        return; 
    }

    fileSim << sig->txFreq << "\n"; 
    fileSim << sig->bufferSize << "\n"; 
    fileSim << sig->oversampleMult << "\n";
    fileSim << sig->sigma << "\n";
    fileSim << sig->txPower << "\n"; 
    fileSim << sig->noiseGain << "\n";
    
    fileSim.close();

    for (bool val : sig->sim_ioBuffer) {
        fileFrame << val; 
    }

    fileFrame.close(); 

    for (double val : sig->sim_samplesBuffer) { 
        fileSamples << val;
    }

    fileSamples.close(); 

    for (double val : sig->radioChannelNoise) { 
        fileChannelNoise << val << "\n"; 
    }

    fileChannelNoise.close(); 

    for (double val : sig->radioChannelReference) { 
        fileChannelReference << val << "\n"; 
    }

    fileChannelReference.close(); 

    for (double val : sig->sim_ioBuffer_ref) { 
        fileBufferDemRef << val; 
    }

    fileBufferDemRef.close(); 

    for (double val : sig->ioBuffer) { 
        fileBufferDem << val; 
    }

    fileBufferDem.close(); 

    for (double val : sig->sim_autoCorrFuncRef) { 
        fileAutoCorrRef << val << "\n"; 
    }

    fileAutoCorrRef.close(); 

    for (double val : sig->sim_autoCorrFunc) { 
        fileAutoCorrFunc << val << "\n"; 
    }

    fileAutoCorrFunc.close(); 
}

void Signal::SetBitWide(size_t oversample) {
    oversampleMult = oversample;
}

void Signal::emit(bool deleteSample) {
    // while (true) {
    //     std::optional<double> result = this->OutBuffer();
        
    //     if (!result.has_value()) {
    //         std::cout << "OutBuffer: nil" << std::endl;
    //         break;
    //     }
        
    //     std::cout << "OutBuffer: " << result.value() << std::endl;
    // }
}

void Signal::printConfig() {
    std::cout << "Signal Configuration:\n";
    std::cout << "Buffer Size: " << bufferSize << "\n";
    std::cout << "RX Frequency: " << rxFreq << "\n";
    std::cout << "TX Frequency: " << txFreq << "\n";
    std::cout << "Modulation type: " << modType << "\n";
}

