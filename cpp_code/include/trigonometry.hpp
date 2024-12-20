#ifndef TRIGONOMETRY_HPP
#define TRIGONOMETRY_HPP

#include <vector>

namespace trigonometry { 
    std::vector<double> generateSineWave(double frequency, double sampleRate, double duration);

    std::vector<double> generateCosineWave(double frequency, double sampleRate, double duration);
}

#endif
