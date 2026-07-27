#ifndef UTILS
#define UTILS

#include <cmath>
#include <cstring>
#include <algorithm>

#if defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2) || defined(__SSE2__)
#include <emmintrin.h>
#define CLOUDREVERB_USE_SSE2_UTILS 1
#else
#define CLOUDREVERB_USE_SSE2_UTILS 0
#endif

namespace CloudSeed
{
class Utils
{
public:
    static inline void ZeroBuffer(double *buffer, int len)
    {
#if CLOUDREVERB_USE_SSE2_UTILS
        const auto zero = _mm_setzero_pd();
        int i = 0;
        for (; i + 1 < len; i += 2)
            _mm_storeu_pd(buffer + i, zero);
        for (; i < len; ++i)
            buffer[i] = 0.0;
#else
        std::fill_n(buffer, len, 0.0);
#endif
    }

    static inline void Copy(const double *source, double *dest, int len)
    {
        std::memcpy(dest, source, len * sizeof(double));
    }

    static inline void Add(const double *source, double *dest, int len)
    {
#if CLOUDREVERB_USE_SSE2_UTILS
        int i = 0;
        for (; i + 1 < len; i += 2)
        {
            const auto a = _mm_loadu_pd(dest + i);
            const auto b = _mm_loadu_pd(source + i);
            _mm_storeu_pd(dest + i, _mm_add_pd(a, b));
        }
        for (; i < len; ++i)
            dest[i] += source[i];
#else
        for (int i = 0; i < len; i++)
        {
            dest[i] += source[i];
        }
#endif
    }

    static inline void Gain(double *buffer, double gain, int len)
    {
#if CLOUDREVERB_USE_SSE2_UTILS
        const auto gainVector = _mm_set1_pd(gain);
        int i = 0;
        for (; i + 1 < len; i += 2)
        {
            const auto x = _mm_loadu_pd(buffer + i);
            _mm_storeu_pd(buffer + i, _mm_mul_pd(x, gainVector));
        }
        for (; i < len; ++i)
            buffer[i] *= gain;
#else
        for (int i = 0; i < len; i++)
        {
            buffer[i] *= gain;
        }
#endif
    }

    // perform bit crushing and undersampling
    // undersampling: if set to 1, perfroms no effect, if set to 2, will undersample to 1/2
    // samplerate, etc... sampleResolution: if set to 32, will use 2^32 steps, if set to 8, will
    // resude to 2^8 = 256 steps Currently Unused
    static inline void BitcrushAndReduce(double *bufferIn, double *bufferOut, int len,
                                         int undersampling, int sampleResolution)
    {
        double sampleSteps = std::pow(2, sampleResolution);
        double inverseSteps = 1.0 / sampleSteps;

        double sample = 0.0;

        for (int i = 0; i < len; i++)
        {
            if (i % undersampling == 0)
                sample = ((long)(bufferIn[i] * sampleSteps)) * inverseSteps;

            bufferOut[i] = sample;
        }
    }

    template <typename T> static double DB2gain(T input) { return std::pow(10, input / 20.0); }

    template <typename T> static double Gain2DB(T input)
    {
        if (input < 0.0000001)
            return -100000;

        return 20.0f * std::log10(input);
    }
};
} // namespace CloudSeed

#endif
