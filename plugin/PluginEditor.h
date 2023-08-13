#pragma once

#include "DiffusionBlock.h"
#include "LateDiffusionBlock.h"
#include "InputBlock.h"
#include "ReverbSlider.h"
#include "TapBlock.h"
#include "DelayBlock.h"
#include "Mixer.h"
#include "EqBlock.h"
#include "RouteBlock.h"
#include "Header.h"
#include "PluginProcessor.h"
#include "MyLookAndFeel.h"

//==============================================================================
class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor,
                                        public juce::ComboBox::Listener
{
public:
    explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;
    // user has changed the preset
    void comboBoxChanged(juce::ComboBox *comboBox) override;
    void restoreUIstate();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor &processorRef;
    MyLookAndFeel myLookAndFeel;
    juce::TextButton reset_button;

    InputBlock input;
    TapBlock tap;
    DelayBlock delay;
    Mixer mixer;
    EqBlock eq;
    DiffusionBlock diffusion1;
    LateDiffusionBlock diffusion2;
    Header header;
    RouteBlock route;

    std::unique_ptr<juce::ComponentBoundsConstrainer> limit;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
