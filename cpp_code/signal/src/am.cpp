#include "signal.hpp"
#include "trigonometry.hpp"

// https://www.nonstopsystems.com/radio/pdf-hell/article-AM-mod-demod.pdf

namespace am { 
    std::vector<double> modulate(std::shared_ptr<Signal> signal, uint bitLevel) { 
        std::vector<bool> *buff = signal->GetBuffer();
        std::vector<double> modulatedWave;
        double carrierFreq = signal->txFreq; // Carrier frequency
        double sampleRate = 1; // Sample rate
        double duration = static_cast<double>(buff->size()) / sampleRate; // Duration of the signal

        for (size_t i = 0; i < buff->size(); ++i) {
            double time = static_cast<double>(i) / sampleRate; // Calculate time for current sample
            double messageAmplitude = (*buff)[i] ? 1.0 : 0.0; // Normalize message to [0, 1] range
            double carrierAmplitude = (1 + messageAmplitude) * 0.5; // Scale to avoid negative values
            double modulatedSample = carrierAmplitude * cos(2 * M_PI * carrierFreq * time); // AM equation
            modulatedWave.push_back(modulatedSample);
        }
        
        return modulatedWave;
    }

    std::vector<double> demodulate(uint64_t carrierFreq, std::vector<double> *channel, uint bitLevel) { 
        std::vector<double> demodulatedSignal;
        double sampleRate = 44100.0;
        size_t numSamples = channel->size();

        // Envelope detection
        for (size_t i = 0; i < numSamples; ++i) {
            // Rectify the signal (take absolute value)
            double rectifiedSample = std::abs((*channel)[i]);
            demodulatedSignal.push_back(rectifiedSample);
        }

        // Low-pass filter to recover original message (basic approach)
        const double alpha = 0.1;
        for (size_t i = 1; i < numSamples; ++i) {
            demodulatedSignal[i] = alpha * demodulatedSignal[i] + (1 - alpha) * demodulatedSignal[i - 1];
        }

        // Normalize to [0, 1]
        double maxVal = *std::max_element(demodulatedSignal.begin(), demodulatedSignal.end());
        for (double& sample : demodulatedSignal) {
            sample /= maxVal;
        }

        return demodulatedSignal; // Return the demodulated signal
    } 
}