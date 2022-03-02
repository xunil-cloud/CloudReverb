#include "RouteBlock.h"
#include <ios>

RouteBlock::RouteBlock(const juce::String &name, const juce::AudioProcessorValueTreeState &state)
    : Block(name)
{
    using type = ReverbSlider::Type;
    addParameter("line count", state.getParameter("LineCount"), type::Circle);

    mode_switch.setButtonText("route mode switch");
    mode_switch.setLookAndFeel(&defaultLook);
    auto param = state.getParameter("LateStageTap");
    attachment_mode_switch = std::make_unique<juce::ButtonParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(param), mode_switch);
    addAndMakeVisible(&mode_switch);
}

RouteBlock::~RouteBlock()
{
    mode_switch.setLookAndFeel(nullptr);
    return;
}

void RouteBlock::resized()
{
    layout();
    mode_switch.setSize(200, 20);
    mode_switch.setCentrePosition(getWidth() * 0.5, getHeight() * 0.5);
}
