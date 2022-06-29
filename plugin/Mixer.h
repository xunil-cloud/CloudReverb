#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

#include "Block.h"
#include "layout/Layout.h"

class Mixer : public Block
{
public:
    Mixer(const juce::String &, const juce::AudioProcessorValueTreeState &);
    void paint(juce::Graphics &) override;
    void resized() override;

private:
    MixerLayout layout;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Mixer)
};
