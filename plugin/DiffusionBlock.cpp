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
    // button.setToggleState();
    auto param = state.getParameter("DiffusionEnabled");
    button_attachment = std::make_unique<juce::ButtonParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(param), button);
    button.setLookAndFeel(&look);
    addAndMakeVisible(button);
}

void DiffusionBlock::resized()
{
    button.setBounds(getWidth() - 35, 5, 30, 30);
    layout();
}
