#include "PluginEditor.h"

#include "PluginProcessor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p) {
    // juce::ignoreUnused(processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    // setResizable(true, true);
    reset_button.setButtonText("reset");
    reset_button.onClick = [this]() {
        auto& state = this->processorRef.treeState;
        state.getParameter("InputMix")->setValueNotifyingHost(0.0);
        state.getParameter("PreDelay")->setValueNotifyingHost(0.070000000298023224);
        state.getParameter("HighPass")->setValueNotifyingHost(0.0);
        state.getParameter("LowPass")->setValueNotifyingHost(0.29000008106231689);
        state.getParameter("TapCount")->setValueNotifyingHost(0.35);
        state.getParameter("TapLength")->setValueNotifyingHost(1.0);
        state.getParameter("TapGain")->setValueNotifyingHost(1.0);
        state.getParameter("TapDecay")->setValueNotifyingHost(0.86500012874603271);
        state.getParameter("DiffusionEnabled")->setValueNotifyingHost(1.0);
        state.getParameter("DiffusionStages")->setValueNotifyingHost(0.4285714328289032);
        state.getParameter("DiffusionDelay")->setValueNotifyingHost(0.43500006198883057);
        state.getParameter("DiffusionFeedback")->setValueNotifyingHost(0.725000262260437);
        state.getParameter("LineCount")->setValueNotifyingHost(1.0);
        state.getParameter("LineDelay")->setValueNotifyingHost(0.68499988317489624);
        state.getParameter("LineDecay")->setValueNotifyingHost(0.68000012636184692);
        state.getParameter("LateDiffusionEnabled")->setValueNotifyingHost(1.0);
        state.getParameter("LateDiffusionStages")->setValueNotifyingHost(0.28571429848670959);
        state.getParameter("LateDiffusionDelay")->setValueNotifyingHost(0.54499995708465576);
        state.getParameter("LateDiffusionFeedback")->setValueNotifyingHost(0.65999996662139893);
        state.getParameter("PostLowShelfGain")->setValueNotifyingHost(0.5199999213218689);
        state.getParameter("PostLowShelfFrequency")->setValueNotifyingHost(0.31499990820884705);
        state.getParameter("PostHighShelfGain")->setValueNotifyingHost(0.83500003814697266);
        state.getParameter("PostHighShelfFrequency")->setValueNotifyingHost(0.73000013828277588);
        state.getParameter("PostCutoffFrequency")->setValueNotifyingHost(0.73499983549118042);
        state.getParameter("EarlyDiffusionModAmount")->setValueNotifyingHost(0.50000005960464478);
        state.getParameter("EarlyDiffusionModRate")->setValueNotifyingHost(0.42500010132789612);
        state.getParameter("LineModAmount")->setValueNotifyingHost(0.59000003337860107);
        state.getParameter("LineModRate")->setValueNotifyingHost(0.46999993920326233);
        state.getParameter("LateDiffusionModAmount")->setValueNotifyingHost(0.619999885559082);
        state.getParameter("LateDiffusionModRate")->setValueNotifyingHost(0.42500019073486328);
        state.getParameter("TapSeed")->setValueNotifyingHost(0.001149);
        state.getParameter("DiffusionSeed")->setValueNotifyingHost(0.000188);
        state.getParameter("DelaySeed")->setValueNotifyingHost(0.000336);
        state.getParameter("PostDiffusionSeed")->setValueNotifyingHost(0.000499);
        state.getParameter("CrossSeed")->setValueNotifyingHost(0.0);
        state.getParameter("DryOut")->setValueNotifyingHost(0.94499987363815308);
        state.getParameter("PredelayOut")->setValueNotifyingHost(0.0);
        state.getParameter("EarlyOut")->setValueNotifyingHost(0.77999997138977051);
        state.getParameter("MainOut")->setValueNotifyingHost(0.74500006437301636);
        state.getParameter("HiPassEnabled")->setValueNotifyingHost(0.0);
        state.getParameter("LowPassEnabled")->setValueNotifyingHost(0.0);
        state.getParameter("LowShelfEnabled")->setValueNotifyingHost(0.0);
        state.getParameter("HighShelfEnabled")->setValueNotifyingHost(0.0);
        state.getParameter("CutoffEnabled")->setValueNotifyingHost(1.0);
        state.getParameter("LateStageTap")->setValueNotifyingHost(1.0);
        state.getParameter("Interpolation")->setValueNotifyingHost(0.);
    };

    for (auto param : this->processorRef.getParameters()) {
        auto name = param->getName(20);

        auto slider = new juce::Slider();
        slider->setSliderStyle(juce::Slider::SliderStyle::Rotary);
        slider->setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
        slider->setPopupDisplayEnabled(true, true, this, -1);

        auto label = new juce::Label("label", param->getName(50));
        label->setJustificationType(juce::Justification::centredTop);

        auto id = dynamic_cast<juce::AudioProcessorParameterWithID*>(param)->paramID;
        sliders.insert({id, slider});
        labels.insert({id, label});
        attachments.push_back(new juce::SliderParameterAttachment(*dynamic_cast<juce::RangedAudioParameter*>(param), *slider));

        addAndMakeVisible(slider);
        addAndMakeVisible(label);
    }
    setSize(800, 600);
    addAndMakeVisible(reset_button);
    addAndMakeVisible(reset_button_);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {
    for (auto i : attachments) {
        delete i;
    }
    for (auto i : sliders) {
        delete i.second;
    }
    for (auto i : labels) {
        delete i.second;
    }
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics& g) {
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colour(100, 100, 100));
    g.drawRect(40 - 10, 40 - 20, 160 + 20, 220 + 30, 2);
    // g.drawRect(40, 40, 160, 220, 2);
    g.setColour(juce::Colours::yellow);
    g.fillEllipse(120, 35, 15, 8);
    g.fillEllipse(120, 145, 12, 8);
}

void AudioPluginAudioProcessorEditor::resized() {
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    //
    reset_button.setBounds(400, 300, 70, 70);

    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    using Px = juce::Grid::Px;
    input_sec.items = {juce::GridItem(sliders["InputMix"]), juce::GridItem(sliders["HighPass"]),
                       juce::GridItem(labels["InputMix"]), juce::GridItem(labels["HighPass"]),
                       juce::GridItem(sliders["PreDelay"]), juce::GridItem(sliders["LowPass"]),
                       juce::GridItem(labels["PreDelay"]), juce::GridItem(labels["LowPass"])};
    input_sec.templateColumns = {Track(Fr(1)), Track(Fr(1))};
    input_sec.templateRows = {Track(Fr(1)), Track(Px(30)), Track(Fr(1)), Track(Px(30))};
    input_sec.alignItems = juce::Grid::AlignItems::end;
    input_sec.performLayout(juce::Rectangle<int>(40, 40, 160, 220));
    // setColour(juce::Slider::ColourIds::textBoxBackgroundColourId, juce::Colour(50, 50, 50));
    // setColour(juce::TooltipWindow, juce::Colours::red);
    early_sec.items = {juce::GridItem(sliders["TapCount"]), juce::GridItem(sliders["TapLength"]), juce::GridItem(sliders["DiffusionDelay"]), juce::GridItem(sliders["DiffusionFeedback"]),
                       juce::GridItem(labels["TapCount"]), juce::GridItem(labels["TapLength"]), juce::GridItem(labels["DiffusionDelay"]), juce::GridItem(labels["DiffusionFeedback"]),
                       juce::GridItem(sliders["TapDecay"]), juce::GridItem(sliders["TapGain"]), juce::GridItem(sliders["EarlyDiffusionModAmount"]), juce::GridItem(sliders["EarlyDiffusionModRate"]),
                       juce::GridItem(labels["TapDecay"]), juce::GridItem(labels["TapGain"]), juce::GridItem(labels["EarlyDiffusionModAmount"]), juce::GridItem(labels["EarlyDiffusionModRate"])};
    early_sec.templateColumns = {Track(Fr(1)), Track(Fr(1)),Track(Fr(1)),Track(Fr(1))};
    early_sec.templateRows = {Track(Fr(1)), Track(Px(30)), Track(Fr(1)), Track(Px(30))};
    early_sec.alignItems = juce::Grid::AlignItems::end;
    early_sec.performLayout(juce::Rectangle<int>(250, 40, 320, 220));
}
