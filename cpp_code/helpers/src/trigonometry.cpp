#include "trigonometry.hpp"
#include <cmath>

namespace trigonometry { 
    std::vector<double> generateSineWave(double frequency, double sampleRate, double duration) {
        std::vector<double> sineWave;
        int sampleCount = static_cast<int>(sampleRate * duration);

        for (int n = 0; n < sampleCount; ++n) {
            double t = n / sampleRate;
            sineWave.push_back(std::sin(2 * M_PI * frequency * t));
        }

        return sineWave;
    }

    std::vector<double> generateCosineWave(double frequency, double sampleRate, double duration) {
        std::vector<double> cosineWave;
        int sampleCount = static_cast<int>(sampleRate * duration);

        for (int n = 0; n < sampleCount; ++n) {
            double t = n / sampleRate;
            cosineWave.push_back(std::cos(2 * M_PI * frequency * t));
        }

        return cosineWave;
    }
}


