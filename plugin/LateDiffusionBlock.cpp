#include "LateDiffusionBlock.h"

LateDiffusionBlock::LateDiffusionBlock(const juce::String &name,
                                       const juce::AudioProcessorValueTreeState &state)
    : Block(name)
{
    using type = ReverbSlider::Type;
    addParameter("stages", state.getParameter("LateDiffusionStages"), type::Circle);
    addParameter("delay", state.getParameter("LateDiffusionDelay"), type::Circle);
    addParameter("feedback", state.getParameter("LateDiffusionFeedback"), type::Circle);
    addParameter("mod amt.", state.getParameter("LateDiffusionModAmount"), type::Circle);
    addParameter("mod rate", state.getParameter("LateDiffusionModRate"), type::Circle);
    setupSeed("seed", state.getParameter("PostDiffusionSeed"));
    // button.setToggleState();
    auto param = state.getParameter("LateDiffusionEnabled");
    button_attachment = std::make_unique<juce::ButtonParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(param), button);
    button.setLookAndFeel(&look);
    addAndMakeVisible(button);
}

void LateDiffusionBlock::resized()
{
    button.setSize(1, 1);
    button.setBoundsToFit(getLocalBounds()
                              .withSizeKeepingCentre(getWidth() * 0.95, getHeight() * 0.9)
                              .removeFromTop(getHeaderSize())
                              .withSizeKeepingCentre(getWidth() * 0.95, getHeaderSize() * 0.7),
                          juce::Justification::centredRight, false);
    layout();
}
