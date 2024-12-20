#include <cmath>
#include "io.hpp"

using namespace std;

namespace io {
    namespace analog {
        int BitsToVal(vector<bool> bits) {
            int val = 0;
            int size = bits.size();

            for (int i = 0; i < size; ++i) {
                if (bits[size - 1 - i]) {
                    val += (1 << i);
                }
            }

            return val;
        }

        vector<double> BitsToSignal(vector<bool> bits, int bitLevel = 2, double maxPower = 1.0) {
            vector<double> signal;

            double step = 2 * maxPower / ((1 << bitLevel) - 1);

            for (size_t i = 0; i < bits.size(); i += bitLevel) {
                vector<bool> subBits;
                for (size_t j = i; j < i + bitLevel && j < bits.size(); ++j) {
                    subBits.push_back(bits[j]);
                }

                int bitVal = BitsToVal(subBits);
                double signalValue = -maxPower + bitVal * step;
                signal.push_back(signalValue);
            }

            return signal;
        }

        vector<bool> SignalToBits(vector<double> samples, int samplesPerBit = 1, int bitLevel = 2, double maxPower = 1.0, double P) {
            vector<bool> bits;

            double step = 2 * maxPower / ((1 << bitLevel) - 1);
            double sum = 0.0;
            int cnt = 0;

            for (size_t i = 0; i < samples.size(); ++i) {
                sum += samples[i];
                ++cnt;

                if (cnt == samplesPerBit) {
                    double avg = sum / cnt;
                    double idealValue = round((avg + maxPower) / step);
                    double actualValue = (avg + maxPower) / step;

                    if (std::abs(actualValue - idealValue) > P) {
                        idealValue = std::max(0.0, std::min((1 << bitLevel) - 1.0, idealValue));
                    }

                    int value = static_cast<int>(idealValue);

                    for (int j = bitLevel - 1; j >= 0; --j) {
                        bits.push_back((value >> j) & 1);
                    }

                    sum = 0.0;
                    cnt = 0;
                }
            }

            return bits;
        }
    }
}
