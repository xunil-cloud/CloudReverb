#include "PluginEditor.h"

#include <array>

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
    auto param_groups = this->processorRef.getParameterTree().getSubgroups(false);
    auto params = param_groups.getFirst()->getParameters(false);

    for (auto param : params) {
        auto name = param->getName(20);

        auto slider = new juce::Slider();
        slider->setLookAndFeel(&myLookAndFeel);
        slider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        slider->setMouseDragSensitivity(300);
        // slider->setVelocityBasedMode(true);
        // slider->setSliderStyle(juce::Slider::SliderStyle::Rotary);
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
    setSize(1552, 600);
    addAndMakeVisible(reset_button);
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
    g.fillAll(juce::Colour(87, 118, 167));
    g.setColour(juce::Colour(100, 100, 100));
    // g.drawRect(40, 40, 11 * 16, 12 * 16);
    // g.drawRect(280 + 16, 40, 25 * 16, 12 * 16);
    for (auto s : sliders) {
        // g.drawRect(s.second->getBounds());
    }
}

void AudioPluginAudioProcessorEditor::resized() {
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    //
    reset_button.setBounds(400, 300, 70, 70);

    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    using Px = juce::Grid::Px;

    std::array<juce::Slider*, 4> input_ui = {sliders["InputMix"], sliders["HighPass"], sliders["PreDelay"], sliders["LowPass"]};
    for (auto i : input_ui) {
        input_sec.items.add(juce::GridItem(i));
    }
    input_sec.templateColumns = {Track(Fr(1)), Track(Fr(1))};
    input_sec.templateRows = {Track(Fr(1)), Track(Fr(1))};
    input_sec.columnGap = Px(48);
    input_sec.rowGap = Px(64);
    input_sec.performLayout(juce::Rectangle<int>(40, 40, 11 * 16, 12 * 16));

    std::array<juce::Slider*, 8> early_ui =
        {sliders["TapCount"], sliders["TapLength"], sliders["DiffusionDelay"], sliders["DiffusionFeedback"],
         sliders["TapDecay"], sliders["TapGain"], sliders["EarlyDiffusionModAmount"], sliders["EarlyDiffusionModRate"]};
    for (auto i : early_ui) {
        early_sec.items.add(juce::GridItem(i));
    }
    early_sec.templateColumns = {Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1))};
    early_sec.templateRows = {Track(Fr(1)), Track(Fr(1))};
    early_sec.columnGap = Px(48);
    early_sec.rowGap = Px(64);
    early_sec.performLayout(juce::Rectangle<int>(296, 40, 25 * 16, 12 * 16));

    std::array<juce::Slider*, 13> late_ui = {
        sliders["LineDelay"],
        sliders["LineDecay"],
        sliders["LineModAmount"],
        sliders["LineModRate"],
        sliders["LateDiffusionDelay"],
        sliders["LateDiffusionFeedback"],
        sliders["LateDiffusionModAmount"],
        sliders["LateDiffusionModRate"],
        sliders["PostLowShelfFrequency"],
        sliders["PostLowShelfGain"],
        sliders["PostHighShelfFrequency"],
        sliders["PostHighShelfGain"],
        sliders["PostCutoffFrequency"]};
    for (auto i : late_ui) {
        late_sec.items.add(juce::GridItem(i));
    }
    late_sec.autoColumns = Track(Fr(1));
    late_sec.templateRows = {Track(Fr(1)), Track(Fr(1))};
    late_sec.autoFlow = juce::Grid::AutoFlow::column;
    late_sec.columnGap = Px(48);
    late_sec.rowGap = Px(64);
    late_sec.performLayout(juce::Rectangle<int>(296 + 25 * 16 + 5 * 16, 40, (7 * 4 + 6 * 3) * 16, 12 * 16));
    for (auto i : labels) {
        auto slider = sliders[i.first];
        auto label = i.second;
        auto fontSize = 20;
        label->setFont(juce::Font(fontSize));
        label->setColour(juce::Label::ColourIds::textColourId, juce::Colour(20, 20, 20));
        label->setMinimumHorizontalScale(1);
        auto width = label->getFont().getStringWidthFloat(label->getText()) + label->getBorderSize().getLeftAndRight() + 1;
        auto height = fontSize + label->getBorderSize().getTopAndBottom() + 1;
        label->setSize(width, height);
        label->setCentrePosition(slider->getPosition().x + slider->getWidth() / 2.0, slider->getPosition().y + slider->getHeight() + label->getHeight() / 2 + 1);
    }
}
