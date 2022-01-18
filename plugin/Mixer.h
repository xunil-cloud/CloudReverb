#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

#include <memory>
#include <unordered_map>

#include "Block.h"
#define DIFFUSION_STAGES "DiffusionStages"

class Mixer : public Block {
public:
    Mixer(const juce::String&, const juce::AudioProcessorValueTreeState&);
    //void paint(juce::Graphics&) override;
    void resized() override;

private:
    const std::vector<juce::String> parameterNames{"DiffusionStages","delay", "feedback", "mod amt.","mod rate" };
    std::unique_ptr<juce::ButtonParameterAttachment> button_attachment;
    MyLookAndFeel look;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Mixer)
};
