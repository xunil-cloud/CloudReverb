#include "DiffusionBlock.h"

DiffusionBlock::DiffusionBlock(const juce::String &name,
                               const juce::AudioProcessorValueTreeState &state)
    : Block(name)
{
    using type = ReverbSlider::Type;
    addParameter("stages", state.getParameter("DiffusionStages"), type::Circle);
    addParameter("delay", state.getParameter("DiffusionDelay"), type::Circle);
    addParameter("feedback", state.getParameter("DiffusionFeedback"), type::Circle);
    addParameter("mod amt.", state.getParameter("EarlyDiffusionModAmount"), type::Circle);
    addParameter("mod rate", state.getParameter("EarlyDiffusionModRate"), type::Circle);
    setupSeed("seed", state.getParameter("DiffusionSeed"));

    auto param = state.getParameter("DiffusionEnabled");
    button_attachment = std::make_unique<juce::ButtonParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(param), button);
    button.setLookAndFeel(&look);
    addAndMakeVisible(button);
}

void DiffusionBlock::resized()
{
    button.setSize(1, 1);
    button.setBoundsToFit(getLocalBounds()
                              .withSizeKeepingCentre(getWidth() * 0.95, getHeight() * 0.9)
                              .removeFromTop(getHeaderSize())
                              .withSizeKeepingCentre(getWidth() * 0.95, getHeaderSize() * 0.7),
                          juce::Justification::centredRight, false);
    layout();
}
