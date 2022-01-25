#include "PluginEditor.h"

#include "PluginProcessor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p), input("Input", this->processorRef.treeState),
      tap("Tap", this->processorRef.treeState), delay("Delay Lines", this->processorRef.treeState),
      mixer("mixer", this->processorRef.treeState), eq("Shelf EQ", this->processorRef.treeState),
      diffusion1("Early Diffusion", this->processorRef.treeState),
      diffusion2("Late Diffusion", this->processorRef.treeState)
{
    reset_button.setButtonText("reset");

    reset_button.onClick = [this]() {
        auto &state = this->processorRef.treeState;
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

    setResizable(true, true);
    setSize(1500, 600);

    limit = std::make_unique<juce::ComponentBoundsConstrainer>();
    limit->setMinimumSize(1100, 450);
    setConstrainer(limit.get());

    addAndMakeVisible(reset_button);
    addAndMakeVisible(input);
    addAndMakeVisible(tap);
    addAndMakeVisible(delay);
    addAndMakeVisible(diffusion1);
    addAndMakeVisible(diffusion2);
    addAndMakeVisible(mixer);
    addAndMakeVisible(eq);
    setLookAndFeel(&myLookAndFeel);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
    return;
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colour(33, 80, 100));
    g.fillRect(getWidth() / 2, 0, 2, getHeight() / 5 * 3);
    g.fillRect(0, getHeight() / 5 * 3 / 2, getWidth(), 2);
    g.fillRect(0, getHeight() / 5 * 3, getWidth(), 2);
}

void AudioPluginAudioProcessorEditor::resized()
{
    reset_button.setBounds(getWidth() - 70, getHeight() - 70, 50, 50);
    auto up = getLocalBounds().removeFromTop(getHeight() * 3 / 5);
    auto down = getLocalBounds().removeFromBottom(getHeight() * 2 / 5);

    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    using Px = juce::Grid::Px;
    grid.templateColumns = {Track(Fr(1)), Track(Fr(1))};
    grid.templateRows = {Track(Fr(1)), Track(Fr(1))};
    grid.items = {juce::GridItem(tap), juce::GridItem(diffusion1), juce::GridItem(delay),
                  juce::GridItem(diffusion2)};
    grid.performLayout(up);
    eq.setBounds(down.removeFromTop(tap.getHeight()));
    // eq.setBounds(down.removeFromTop(down.getHeight() / 2));
}
