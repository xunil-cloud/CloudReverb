#ifndef MULTITAPDIFFUSER
#define MULTITAPDIFFUSER

#include <algorithm>
#include <vector>
#include <memory>
#include <array>
#include <cassert>

#include "Utils.h"
#include "AudioLib/ShaRandom.h"

namespace CloudSeed
{
using namespace std;

class MultitapDiffuser
{
public:
    static const int MaxTaps = 50;

private:
    double *buffer;
    double *output;
    // int len;
    int bufferSize;
    int maxDelaySamples;

    int index;
    vector<double> tapGains;
    vector<int> tapPosition;
    vector<double> tapData;
    vector<double> seedValues;
    int seed;
    double crossSeed;
    int count;
    double length;
    double gain;
    double decay;

public:
    MultitapDiffuser(int delayBufferSize)
    {
        bufferSize = delayBufferSize;
        maxDelaySamples = delayBufferSize;
        buffer = new double[delayBufferSize];
        output = new double[delayBufferSize];
        tapGains.resize(50);
        tapPosition.resize(50);
        tapData.resize(50);

        index = 0;
        count = 1;
        length = 1;
        gain = 1.0;
        decay = 0.0;
        crossSeed = 0.0;
        UpdateSeeds();
    }

    ~MultitapDiffuser()
    {
        delete[] buffer;
        delete[] output;
    }

    void SetSeed(int seed)
    {
        this->seed = seed;
        UpdateSeeds();
    }

    void SetCrossSeed(double crossSeed)
    {
        this->crossSeed = crossSeed;
        UpdateSeeds();
    }

    double *GetOutput() { return output; }

    void SetTapCount(int tapCount)
    {
        count = tapCount;
        Update();
    }

    void SetTapLength(int tapLength)
    {
        length = tapLength;
        Update();
    }

    void SetTapDecay(double tapDecay)
    {
        decay = tapDecay;
        Update();
    }

    void SetTapGain(double tapGain)
    {
        gain = tapGain;
        Update();
    }

    void Process(double *input, int sampleCount)
    {
        for (int i = 0; i < sampleCount; i++)
        {
            if (index < 0)
                index += maxDelaySamples;
            assert(index < maxDelaySamples);
            buffer[index] = input[i];
            output[i] = 0.0;

            for (int j = 0; j < count; j++)
            {
                auto idx = (index + tapPosition[j]) % maxDelaySamples;
                output[i] += buffer[idx] * tapGains[j];
            }

            index--;
        }
    }

    void ClearBuffers()
    {
        Utils::ZeroBuffer(buffer, maxDelaySamples);
        Utils::ZeroBuffer(output, bufferSize);
    }
    void prepare(int sampleRate, int bufferSize)
    {
        this->bufferSize = bufferSize;
        maxDelaySamples = sampleRate; // 1 second delay
        delete[] output;
        delete[] buffer;
        output = new double[bufferSize];
        buffer = new double[maxDelaySamples];
        Utils::ZeroBuffer(output, bufferSize);
        Utils::ZeroBuffer(buffer, maxDelaySamples);
        index = 0;
    }

private:
    void Update()
    {
        int s = 0;
        auto rand = [&]() { return seedValues[s++]; };

        if (count < 1)
            count = 1;

        if (length < count)
            length = count;

        // used to adjust the volume of the overall output as it grows when we add more taps
        double tapCountFactor = 1.0 / (1 + std::sqrt(count / MaxTaps));

        auto sumLengths = 0.0;
        for (size_t i = 0; i < count; i++)
        {
            auto val = 0.1 + rand();
            tapData[i] = val;
            sumLengths += val;
        }

        auto scaleLength = length / sumLengths;
        tapPosition[0] = 0;

        for (int i = 1; i < count; i++)
        {
            tapPosition[i] = tapPosition[i - 1] + (int)(tapData[i] * scaleLength);
        }

        double sumGains = 0.0;
        double lastTapPos = tapPosition[count - 1];
        for (int i = 0; i < count; i++)
        {
            // when decay set to 0, there is no decay, when set to 1, the gain at the last sample is
            // 0.01 = -40dB
            auto g = std::pow(10, -decay * 2 * tapPosition[i] / (double)(lastTapPos + 1));

            auto tap = (2 * rand() - 1) * tapCountFactor;
            tapGains[i] = tap * g * gain;
        }

        // Set the tap vs. clean mix
        tapGains[0] = (1 - gain);
    }

    void UpdateSeeds()
    {
        this->seedValues = AudioLib::ShaRandom::Generate(seed, 100, crossSeed);
        Update();
    }
};
} // namespace CloudSeed

#endif
