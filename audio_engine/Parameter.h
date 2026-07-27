#ifndef PARAMETER
#define PARAMETER

enum class Parameter
{
    // Input

    InputMix = 0,
    PreDelay,

    HighPass,
    LowPass,

    // Early

    TapEnabled,
    TapCount,
    TapLength,
    TapGain,
    TapDecay,

    DiffusionEnabled,
    DiffusionStages,
    DiffusionDelay,
    DiffusionFeedback,

    // Late

    LateDelayEnabled,
    LineCount,
    LineDelay,
    LineDecay,

    LateDiffusionEnabled,
    LateDiffusionStages,
    LateDiffusionDelay,
    LateDiffusionFeedback,

    // Frequency Response

    PostLowShelfGain,
    PostLowShelfFrequency,
    PostHighShelfGain,
    PostHighShelfFrequency,
    PostCutoffFrequency,

    // Modulation

    EarlyDiffusionModAmount,
    EarlyDiffusionModRate,

    LineModAmount,
    LineModRate,

    LateDiffusionModAmount,
    LateDiffusionModRate,

    // Seeds

    TapSeed,
    DiffusionSeed,
    DelaySeed,
    PostDiffusionSeed,

    // Seed Mix

    CrossSeed,

    DryOut,
    PredelayOut,
    EarlyOut,
    MainOut,
    WidthOut,

    // Switches
    HiPassEnabled,
    LowPassEnabled,
    LowShelfEnabled,
    HighShelfEnabled,
    CutoffEnabled,
    LateStageTap,

    // Effects
    Interpolation,

    Count,

    Unused = 999
};

#endif
