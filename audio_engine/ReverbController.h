
#ifndef REVERBCONTROLLER
#define REVERBCONTROLLER

#ifdef _MSC_VER
#pragma warning(disable: 4702 4244 4305 4458 4100)
#endif

#include <vector>
#include "Parameter.h"
#include "ReverbChannel.h"

#include "AudioLib/ValueTables.h"
#include "AllpassDiffuser.h"
#include "MultitapDiffuser.h"
#include "Utils.h"

#if CLOUDREVERB_USE_SSE2_UTILS
#include <xmmintrin.h>
#include <emmintrin.h>
#endif

namespace CloudSeed
{
class ReverbController
{
private:
    int bufferSize = 128;
    int samplerate;

    ReverbChannel channelL;
    ReverbChannel channelR;
    double *leftChannelIn;
    double *rightChannelIn;
    // double leftLineBuffer[bufferSize];
    // double rightLineBuffer[bufferSize];
    double parameters[(int)Parameter::Count];
    double stereoWidth{1.0};

public:
    ReverbController(int samplerate)
        : channelL(bufferSize, samplerate, ChannelLR::Left),
          channelR(bufferSize, samplerate, ChannelLR::Right)
    {
        this->samplerate = samplerate;
        leftChannelIn = new double[bufferSize];
        rightChannelIn = new double[bufferSize];
    }
    ~ReverbController()
    {
        delete[] leftChannelIn;
        delete[] rightChannelIn;
    }

    int GetSamplerate() { return samplerate; }

    void SetSamplerate(int samplerate)
    {
        this->samplerate = samplerate;

        channelL.SetSamplerate(samplerate);
        channelR.SetSamplerate(samplerate);
    }

    int GetParameterCount() { return (int)Parameter::Count; }

    double *GetAllParameters() { return parameters; }

    double GetScaledParameter(Parameter param)
    {
        switch (param)
        {
            // Input
        case Parameter::InputMix:
            return P(Parameter::InputMix);
        case Parameter::PreDelay:
            return (int)(P(Parameter::PreDelay) * 1000);

        case Parameter::HighPass:
            return 20 + ValueTables::Get(P(Parameter::HighPass), ValueTables::Response4Oct) * 980;
        case Parameter::LowPass:
            return 400 + ValueTables::Get(P(Parameter::LowPass), ValueTables::Response4Oct) * 19600;

            // Early
        case Parameter::TapEnabled:
            return P(Parameter::TapEnabled) < 0.5 ? 0.0 : 1.0;
        case Parameter::TapCount:
            return 1 + (int)(P(Parameter::TapCount) * (MultitapDiffuser::MaxTaps - 1));
        case Parameter::TapLength:
            return (int)(P(Parameter::TapLength) * 500);
        case Parameter::TapGain:
            return ValueTables::Get(P(Parameter::TapGain), ValueTables::Response2Dec);
        case Parameter::TapDecay:
            return P(Parameter::TapDecay);

        case Parameter::DiffusionEnabled:
            return P(Parameter::DiffusionEnabled) < 0.5 ? 0.0 : 1.0;
        case Parameter::DiffusionStages:
            return 1 +
                   (int)(P(Parameter::DiffusionStages) * (AllpassDiffuser::MaxStageCount - 0.001));
        case Parameter::DiffusionDelay:
            return (int)(10 + P(Parameter::DiffusionDelay) * 90);
        case Parameter::DiffusionFeedback:
            return P(Parameter::DiffusionFeedback);

            // Late
        case Parameter::LateDelayEnabled:
            return P(Parameter::LateDelayEnabled) < 0.5 ? 0.0 : 1.0;
        case Parameter::LineCount:
            return 1 + (int)(P(Parameter::LineCount) * 11.999);
        case Parameter::LineDelay:
            return (int)(20.0 +
                         ValueTables::Get(P(Parameter::LineDelay), ValueTables::Response2Dec) *
                             980);
        case Parameter::LineDecay:
            return 0.05 +
                   ValueTables::Get(P(Parameter::LineDecay), ValueTables::Response3Dec) * 59.95;

        case Parameter::LateDiffusionEnabled:
            return P(Parameter::LateDiffusionEnabled) < 0.5 ? 0.0 : 1.0;
        case Parameter::LateDiffusionStages:
            return 1 + (int)(P(Parameter::LateDiffusionStages) *
                             (AllpassDiffuser::MaxStageCount - 0.001));
        case Parameter::LateDiffusionDelay:
            return (int)(10 + P(Parameter::LateDiffusionDelay) * 90);
        case Parameter::LateDiffusionFeedback:
            return P(Parameter::LateDiffusionFeedback);

            // Frequency Response
        case Parameter::PostLowShelfGain:
            return ValueTables::Get(P(Parameter::PostLowShelfGain), ValueTables::Response2Dec);
        case Parameter::PostLowShelfFrequency:
            return 20 + ValueTables::Get(P(Parameter::PostLowShelfFrequency),
                                         ValueTables::Response4Oct) *
                            980;
        case Parameter::PostHighShelfGain:
            return ValueTables::Get(P(Parameter::PostHighShelfGain), ValueTables::Response2Dec);
        case Parameter::PostHighShelfFrequency:
            return 400 + ValueTables::Get(P(Parameter::PostHighShelfFrequency),
                                          ValueTables::Response4Oct) *
                             19600;
        case Parameter::PostCutoffFrequency:
            return 400 +
                   ValueTables::Get(P(Parameter::PostCutoffFrequency), ValueTables::Response4Oct) *
                       19600;

            // Modulation
        case Parameter::EarlyDiffusionModAmount:
            return P(Parameter::EarlyDiffusionModAmount) * 2.5;
        case Parameter::EarlyDiffusionModRate:
            return ValueTables::Get(P(Parameter::EarlyDiffusionModRate),
                                    ValueTables::Response2Dec) *
                   5;
        case Parameter::LineModAmount:
            return P(Parameter::LineModAmount) * 2.5;
        case Parameter::LineModRate:
            return ValueTables::Get(P(Parameter::LineModRate), ValueTables::Response2Dec) * 5;
        case Parameter::LateDiffusionModAmount:
            return P(Parameter::LateDiffusionModAmount) * 2.5;
        case Parameter::LateDiffusionModRate:
            return ValueTables::Get(P(Parameter::LateDiffusionModRate), ValueTables::Response2Dec) *
                   5;

            // Seeds
        case Parameter::TapSeed:
            return (int)std::floor(P(Parameter::TapSeed) * 1000000 + 0.001);
        case Parameter::DiffusionSeed:
            return (int)std::floor(P(Parameter::DiffusionSeed) * 1000000 + 0.001);
        case Parameter::DelaySeed:
            return (int)std::floor(P(Parameter::DelaySeed) * 1000000 + 0.001);
        case Parameter::PostDiffusionSeed:
            return (int)std::floor(P(Parameter::PostDiffusionSeed) * 1000000 + 0.001);

            // Output
        case Parameter::CrossSeed:
            return P(Parameter::CrossSeed);

        case Parameter::DryOut:
            return ValueTables::Get(P(Parameter::DryOut), ValueTables::Response2Dec);
        case Parameter::PredelayOut:
            return ValueTables::Get(P(Parameter::PredelayOut), ValueTables::Response2Dec);
        case Parameter::EarlyOut:
            return ValueTables::Get(P(Parameter::EarlyOut), ValueTables::Response2Dec);
        case Parameter::MainOut:
            return ValueTables::Get(P(Parameter::MainOut), ValueTables::Response2Dec);
        case Parameter::WidthOut:
            return P(Parameter::WidthOut);

            // Switches
        case Parameter::HiPassEnabled:
            return P(Parameter::HiPassEnabled) < 0.5 ? 0.0 : 1.0;
        case Parameter::LowPassEnabled:
            return P(Parameter::LowPassEnabled) < 0.5 ? 0.0 : 1.0;
        case Parameter::LowShelfEnabled:
            return P(Parameter::LowShelfEnabled) < 0.5 ? 0.0 : 1.0;
        case Parameter::HighShelfEnabled:
            return P(Parameter::HighShelfEnabled) < 0.5 ? 0.0 : 1.0;
        case Parameter::CutoffEnabled:
            return P(Parameter::CutoffEnabled) < 0.5 ? 0.0 : 1.0;
        case Parameter::LateStageTap:
            return P(Parameter::LateStageTap) < 0.5 ? 0.0 : 1.0;

            // Effects
        case Parameter::Interpolation:
            return P(Parameter::Interpolation) < 0.5 ? 0.0 : 1.0;

        default:
            return 0.0;
        }

        return 0.0;
    }

    void SetParameter(Parameter param, double value)
    {
        parameters[(int)param] = value;
        auto scaled = GetScaledParameter(param);

        if (param == Parameter::WidthOut)
        {
            stereoWidth = scaled;
            return;
        }

        channelL.SetParameter(param, scaled);
        channelR.SetParameter(param, scaled);
    }
    void updateParameter(Parameter param, double scaled_value, float normalized_value)
    {
        parameters[(int)param] = normalized_value;

        if (param == Parameter::WidthOut)
        {
            stereoWidth = scaled_value;
            return;
        }

        channelL.SetParameter(param, scaled_value);
        channelR.SetParameter(param, scaled_value);
    }

    void ClearBuffers()
    {
        channelL.ClearBuffers();
        channelR.ClearBuffers();
    }

    void Process(const float *const *input, float *const *output, int bufferSize)
    {
        auto len = bufferSize;
        auto cm = GetScaledParameter(Parameter::InputMix) * 0.5;
        auto cmi = (1 - cm);

#if CLOUDREVERB_USE_SSE2_UTILS
        const auto cmv = _mm_set1_pd(cm);
        const auto cmiv = _mm_set1_pd(cmi);
        int i = 0;

        for (; i + 3 < len; i += 4)
        {
            const auto l4 = _mm_loadu_ps(input[0] + i);
            const auto r4 = _mm_loadu_ps(input[1] + i);

            const auto l01 = _mm_cvtps_pd(l4);
            const auto r01 = _mm_cvtps_pd(r4);
            const auto l23 = _mm_cvtps_pd(_mm_movehl_ps(l4, l4));
            const auto r23 = _mm_cvtps_pd(_mm_movehl_ps(r4, r4));

            const auto left01 = _mm_add_pd(_mm_mul_pd(l01, cmiv), _mm_mul_pd(r01, cmv));
            const auto right01 = _mm_add_pd(_mm_mul_pd(r01, cmiv), _mm_mul_pd(l01, cmv));
            const auto left23 = _mm_add_pd(_mm_mul_pd(l23, cmiv), _mm_mul_pd(r23, cmv));
            const auto right23 = _mm_add_pd(_mm_mul_pd(r23, cmiv), _mm_mul_pd(l23, cmv));

            _mm_storeu_pd(leftChannelIn + i, left01);
            _mm_storeu_pd(rightChannelIn + i, right01);
            _mm_storeu_pd(leftChannelIn + i + 2, left23);
            _mm_storeu_pd(rightChannelIn + i + 2, right23);
        }

        for (; i < len; ++i)
        {
            leftChannelIn[i] = input[0][i] * cmi + input[1][i] * cm;
            rightChannelIn[i] = input[1][i] * cmi + input[0][i] * cm;
        }
#else
        for (int i = 0; i < len; i++)
        {
            leftChannelIn[i] = input[0][i] * cmi + input[1][i] * cm;
            rightChannelIn[i] = input[1][i] * cmi + input[0][i] * cm;
        }
#endif

        channelL.Process(leftChannelIn, len);
        channelR.Process(rightChannelIn, len);
        auto leftOut = channelL.GetOutput();
        auto rightOut = channelR.GetOutput();

        const auto width = stereoWidth;
        for (int i = 0; i < len; i++)
        {
            const auto left = leftOut[i];
            const auto right = rightOut[i];
            const auto mid = (left + right) * 0.5;
            const auto side = (left - right) * 0.5 * width;
            output[0][i] = (float)(mid + side);
            output[1][i] = (float)(mid - side);
        }
    }

    void ProcessMono(const float *const *input, float *const *output, int bufferSize)
    {
        auto len = bufferSize;

        for (int i = 0; i < len; i++)
        {
            leftChannelIn[i] = input[0][i];
        }

        channelL.Process(leftChannelIn, len);
        channelR.ClearBuffers();
        auto leftOut = channelL.GetOutput();

        for (int i = 0; i < len; i++)
        {
            output[0][i] = leftOut[i];
        }
    }
    void prepare(int sampleRate, int bufferSize)
    {
        const auto bufferSizeChanged = this->bufferSize != bufferSize;
        this->bufferSize = bufferSize;

        channelL.prepare(sampleRate, bufferSize);
        channelR.prepare(sampleRate, bufferSize);

        if (bufferSizeChanged)
        {
            delete[] leftChannelIn;
            delete[] rightChannelIn;
            leftChannelIn = new double[bufferSize];
            rightChannelIn = new double[bufferSize];
        }

        Utils::ZeroBuffer(leftChannelIn, bufferSize);
        Utils::ZeroBuffer(rightChannelIn, bufferSize);
    }

private:
    double P(Parameter para)
    {
        auto idx = (int)para;
        return idx >= 0 && idx < (int)Parameter::Count ? parameters[idx] : 0.0;
    }
};
} // namespace CloudSeed
#endif
