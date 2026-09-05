#include "../audio_engine/ReverbController.h"
#include "../audio_engine/Parameter.h"
#include "../plugin/presets/presets.h"
#include <cstdio>

#define APPLY_PARAM_FLOAT(name)                                                                    \
    {                                                                                              \
        reverb.SetParameter(Parameter::name, data[#name]);                                         \
        auto name##Value = reverb.GetScaledParameter(Parameter::name);                             \
        printf(#name ": %f\n", reverb.GetScaledParameter(Parameter::name));                        \
    }
#define APPLY_PARAM_INT(name)                                                                      \
    {                                                                                              \
        reverb.SetParameter(Parameter::name, data[#name]);                                         \
        auto name##Value = reverb.GetScaledParameter(Parameter::name);                             \
        printf(#name ": %d\n", (int)(double)(int)reverb.GetScaledParameter(Parameter::name));      \
    }

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
void dump_preset(CloudSeed::ReverbController &reverb, const std::string &filename)
{

    // Open the JSON file
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file\n";
        exit(1);
    }

    // Parse JSON
    json data;
    try
    {
        file >> data; // automatic parsing
    }
    catch (const json::parse_error &e)
    {
        std::cerr << "JSON parse error: " << e.what() << "\n";
        exit(1);
    }
    reverb.SetParameter(Parameter::DryOut, data["DryOut"]);
    reverb.SetParameter(Parameter::PredelayOut, data["PredelayOut"]);
    reverb.SetParameter(Parameter::EarlyOut, data["EarlyOut"]);
    reverb.SetParameter(Parameter::MainOut, data["MainOut"]);

    auto dryout = reverb.GetScaledParameter(Parameter::DryOut);
    auto predelayout = reverb.GetScaledParameter(Parameter::PredelayOut);
    auto earlyout = reverb.GetScaledParameter(Parameter::EarlyOut);
    auto mainout = reverb.GetScaledParameter(Parameter::MainOut);

    auto map = [](float value) {
        return std::log((value - 0) / (1 - 0) * (10 - 1) + 1) / std ::log(10);
    };
    printf("Dryout: %f (%f dB), %f\n", dryout, CloudSeed::Utils::Gain2DB(dryout), map(dryout));
    printf("PredelayOut: %f (%f dB), %f\n", predelayout, CloudSeed::Utils::Gain2DB(predelayout),
           map(predelayout));
    printf("EarlyOut: %f (%f dB), %f\n", earlyout, CloudSeed::Utils::Gain2DB(earlyout),
           map(earlyout));
    printf("MainOut: %f (%f dB), %f\n", mainout, CloudSeed::Utils::Gain2DB(mainout), map(mainout));

    APPLY_PARAM_INT(TapCount)
    APPLY_PARAM_INT(TapLength)
    APPLY_PARAM_FLOAT(TapDecay)
    APPLY_PARAM_FLOAT(TapGain)
    APPLY_PARAM_FLOAT(TapSeed)
    printf("\n");

    APPLY_PARAM_INT(DiffusionStages)
    APPLY_PARAM_INT(DiffusionDelay)
    APPLY_PARAM_FLOAT(DiffusionFeedback)
    APPLY_PARAM_FLOAT(EarlyDiffusionModAmount)
    APPLY_PARAM_FLOAT(EarlyDiffusionModRate)
    APPLY_PARAM_INT(DiffusionEnabled)
    APPLY_PARAM_INT(DiffusionSeed)

    printf("\n");

    APPLY_PARAM_INT(LateDiffusionStages)
    APPLY_PARAM_INT(LateDiffusionDelay)
    APPLY_PARAM_FLOAT(LateDiffusionFeedback)
    APPLY_PARAM_FLOAT(LateDiffusionModAmount)
    APPLY_PARAM_FLOAT(LateDiffusionModRate)
    APPLY_PARAM_INT(LateDiffusionEnabled)
    APPLY_PARAM_INT(PostDiffusionSeed)

    printf("\n");

    APPLY_PARAM_FLOAT(LineDelay)
    auto mapp = [](float start, float end, float value) {
        return std ::log((value - start) / (end - start) * (100 - 1) + 1) / std ::log(100);
    };

    auto delay = reverb.GetScaledParameter(Parameter::LineDelay);
    // printf("---- LineDelay: %f\n", mapp(20, 1000, delay));
    APPLY_PARAM_FLOAT(LineDecay)
    APPLY_PARAM_FLOAT(LineModAmount)
    APPLY_PARAM_FLOAT(LineModRate)
    APPLY_PARAM_INT(LineCount)
    APPLY_PARAM_INT(DelaySeed)

    printf("\n");

    APPLY_PARAM_FLOAT(PostLowShelfFrequency)
    APPLY_PARAM_FLOAT(PostLowShelfGain)
    APPLY_PARAM_FLOAT(LowShelfEnabled)
    APPLY_PARAM_FLOAT(PostHighShelfFrequency)
    APPLY_PARAM_FLOAT(PostHighShelfGain)
    APPLY_PARAM_FLOAT(HighShelfEnabled)
}
int main(int argc, char **argv)
{
    if(argc == 1) {
        printf("usage: dump_presets <preset json file>\n");
        exit(0);
    }

    AudioLib::ValueTables::Init();
    CloudSeed::FastSin::Init();
    CloudSeed::ReverbController c{48000};
    c.prepare(48000, 1024);
    c.SetSamplerate(48000);
    printf("***** %s *****\n", argv[1]);
    dump_preset(c, argv[1]);
}
