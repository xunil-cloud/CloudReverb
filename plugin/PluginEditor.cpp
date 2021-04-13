#include "PluginEditor.h"

#include "PluginProcessor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p), attachment(*dynamic_cast<AudioPluginAudioProcessor*>(&p)->treeState.getParameter("PreDelay"), slider) {
    // juce::ignoreUnused(processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(800, 600);
    addAndMakeVisible(slider);
    reset_button.setButtonText("reset");
    addAndMakeVisible(reset_button);
    reset_button.onClick = [this]() {
        this->processorRef.treeState.getParameter("InputMix")->setValueNotifyingHost(0.0);
        this->processorRef.treeState.getParameter("PreDelay")->setValueNotifyingHost(0.070000000298023224);
        this->processorRef.treeState.getParameter("HighPass")->setValueNotifyingHost(0.0);
        this->processorRef.treeState.getParameter("LowPass")->setValueNotifyingHost(0.29000008106231689);
        this->processorRef.treeState.getParameter("TapCount")->setValueNotifyingHost(0.35);
        this->processorRef.treeState.getParameter("TapLength")->setValueNotifyingHost(1.0);
        this->processorRef.treeState.getParameter("TapGain")->setValueNotifyingHost(1.0);
        this->processorRef.treeState.getParameter("TapDecay")->setValueNotifyingHost(0.86500012874603271);
        this->processorRef.treeState.getParameter("DiffusionEnabled")->setValueNotifyingHost(1.0);
        this->processorRef.treeState.getParameter("DiffusionStages")->setValueNotifyingHost(0.4285714328289032);
        this->processorRef.treeState.getParameter("DiffusionDelay")->setValueNotifyingHost(0.43500006198883057);
        this->processorRef.treeState.getParameter("DiffusionFeedback")->setValueNotifyingHost(0.725000262260437);
        this->processorRef.treeState.getParameter("LineCount")->setValueNotifyingHost(1.0);
        this->processorRef.treeState.getParameter("LineDelay")->setValueNotifyingHost(0.68499988317489624);
        this->processorRef.treeState.getParameter("LineDecay")->setValueNotifyingHost(0.68000012636184692);
        this->processorRef.treeState.getParameter("LateDiffusionEnabled")->setValueNotifyingHost(1.0);
        this->processorRef.treeState.getParameter("LateDiffusionStages")->setValueNotifyingHost(0.28571429848670959);
        this->processorRef.treeState.getParameter("LateDiffusionDelay")->setValueNotifyingHost(0.54499995708465576);
        this->processorRef.treeState.getParameter("LateDiffusionFeedback")->setValueNotifyingHost(0.65999996662139893);
        this->processorRef.treeState.getParameter("PostLowShelfGain")->setValueNotifyingHost(0.5199999213218689);
        this->processorRef.treeState.getParameter("PostLowShelfFrequency")->setValueNotifyingHost(0.31499990820884705);
        this->processorRef.treeState.getParameter("PostHighShelfGain")->setValueNotifyingHost(0.83500003814697266);
        this->processorRef.treeState.getParameter("PostHighShelfFrequency")->setValueNotifyingHost(0.73000013828277588);
        this->processorRef.treeState.getParameter("PostCutoffFrequency")->setValueNotifyingHost(0.73499983549118042);
        this->processorRef.treeState.getParameter("EarlyDiffusionModAmount")->setValueNotifyingHost(0.50000005960464478);
        this->processorRef.treeState.getParameter("EarlyDiffusionModRate")->setValueNotifyingHost(0.42500010132789612);
        this->processorRef.treeState.getParameter("LineModAmount")->setValueNotifyingHost(0.59000003337860107);
        this->processorRef.treeState.getParameter("LineModRate")->setValueNotifyingHost(0.46999993920326233);
        this->processorRef.treeState.getParameter("LateDiffusionModAmount")->setValueNotifyingHost(0.619999885559082);
        this->processorRef.treeState.getParameter("LateDiffusionModRate")->setValueNotifyingHost(0.42500019073486328);
        this->processorRef.treeState.getParameter("TapSeed")->setValueNotifyingHost(0.001149);
        this->processorRef.treeState.getParameter("DiffusionSeed")->setValueNotifyingHost(0.000188);
        this->processorRef.treeState.getParameter("DelaySeed")->setValueNotifyingHost(0.000336);
        this->processorRef.treeState.getParameter("PostDiffusionSeed")->setValueNotifyingHost(0.000499);
        this->processorRef.treeState.getParameter("CrossSeed")->setValueNotifyingHost(0.0);
        this->processorRef.treeState.getParameter("DryOut")->setValueNotifyingHost(0.94499987363815308);
        this->processorRef.treeState.getParameter("PredelayOut")->setValueNotifyingHost(0.0);
        this->processorRef.treeState.getParameter("EarlyOut")->setValueNotifyingHost(0.77999997138977051);
        this->processorRef.treeState.getParameter("MainOut")->setValueNotifyingHost(0.74500006437301636);
        this->processorRef.treeState.getParameter("HiPassEnabled")->setValueNotifyingHost(0.0);
        this->processorRef.treeState.getParameter("LowPassEnabled")->setValueNotifyingHost(0.0);
        this->processorRef.treeState.getParameter("LowShelfEnabled")->setValueNotifyingHost(0.0);
        this->processorRef.treeState.getParameter("HighShelfEnabled")->setValueNotifyingHost(0.0);
        this->processorRef.treeState.getParameter("CutoffEnabled")->setValueNotifyingHost(1.0);
        this->processorRef.treeState.getParameter("LateStageTap")->setValueNotifyingHost(1.0);
        this->processorRef.treeState.getParameter("Interpolation")->setValueNotifyingHost(0.);
    };
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics& g) {
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
}

void AudioPluginAudioProcessorEditor::resized() {
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    slider.setBounds(10, 10, 300, 300);
    reset_button.setBounds(500, 500, 80, 80);
}
