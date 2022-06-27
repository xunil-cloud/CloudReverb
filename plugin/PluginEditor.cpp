#include "PluginEditor.h"

#include "Block.h"
#include "PluginProcessor.h"
#include <memory>

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p), input("Input", this->processorRef.treeState),
      tap("Multitap Delay", this->processorRef.treeState), delay("Late Delay", this->processorRef.treeState),
      mixer("Mixer", this->processorRef.treeState), eq("Shelf EQ", this->processorRef.treeState),
      diffusion1("Early Diffusion", this->processorRef.treeState),
      diffusion2("Late Diffusion", this->processorRef.treeState),
      header("CloudReverb", this->processorRef.treeState),
      route("Route", this->processorRef.treeState)
{
    reset_button.setButtonText("R");

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

    combobox.addItem("preset 1", 1);
    combobox.addItem("preset 2", 2);
    combobox.setSelectedId(1);
    const auto displayWidth =
        juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()->totalArea.getWidth();
    const auto displayHeight =
        juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()->totalArea.getHeight();
    const auto userRatio = std::min(displayWidth / 1920.f, displayHeight / 1080.f);
    const auto defaultWidth = 1418.f;
    const auto defaultHeight = 782.f;

    limit = std::make_unique<juce::ComponentBoundsConstrainer>();
    limit->setMinimumSize(defaultWidth * 0.9 * userRatio * 0.4,
                          defaultHeight * 0.9 * userRatio * 0.4);
    limit->setFixedAspectRatio(defaultWidth / defaultHeight);
    setConstrainer(limit.get());
    setSize(defaultWidth * 0.9 * userRatio, defaultHeight * 0.9 * userRatio);

    addAndMakeVisible(reset_button);
    addAndMakeVisible(input);
    addAndMakeVisible(tap);
    addAndMakeVisible(delay);
    addAndMakeVisible(diffusion1);
    addAndMakeVisible(diffusion2);
    addAndMakeVisible(mixer);
    addAndMakeVisible(eq);
    addAndMakeVisible(route);
    addAndMakeVisible(header);
    setLookAndFeel(&myLookAndFeel);

    setResizable(true, true);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() { setLookAndFeel(nullptr); }

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void AudioPluginAudioProcessorEditor::resized()
{
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    using Px = juce::Grid::Px;

    grid.templateColumns = {Track(Fr(540)), Track(Fr(540)), Track(Fr(330))};
    grid.templateRows = {Track(Fr(1)), Track(Fr(2)), Track(Fr(2)), Track(Fr(2))};
    grid.columnGap = Px(4);
    grid.rowGap = Px(4);
    grid.items = {juce::GridItem(header).withArea(1, 1, 1, 4),
                  juce::GridItem(tap),
                  juce::GridItem(diffusion1),
                  juce::GridItem(eq).withArea(2, 3, 4, 3),
                  juce::GridItem(delay),
                  juce::GridItem(diffusion2),
                  juce::GridItem(input),
                  juce::GridItem(route),
                  juce::GridItem(mixer)};
    grid.performLayout(getLocalBounds());

    auto bound =
        eq.getBounds()
            .withHeight(getHeight() * 0.08)
            .withPosition(eq.getBounds().getX(), 0)
            .withSizeKeepingCentre(eq.getWidth() * 0.9, std::min(getHeight() * 0.08 * 0.6, 25.0));
    reset_button.setBounds(bound.withTrimmedLeft(bound.getWidth() - 35));
}
