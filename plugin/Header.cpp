#include "Header.h"

Header::Header(const juce::String &name, const juce::AudioProcessorValueTreeState &state)
    : Block(name), cross_seed_slider("cross seed", ReverbSlider::Type::Circle)
{
    setInterceptsMouseClicks(false, true);

    interp_switch.setName("interpolation");
    auto param = state.getParameter("Interpolation");
    attachment_interp_switch = std::make_unique<juce::ButtonParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(param), interp_switch);

    param = state.getParameter("CrossSeed");
    attachment_cross_seed = std::make_unique<juce::SliderParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(param), cross_seed_slider);

    cross_seed_slider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(interp_switch);
    addAndMakeVisible(cross_seed_slider);
}
void Header::paint(juce::Graphics &g)
{
    layout.drawTitle(g, getName(), getLocalBounds());
    layout.drawTextRightToSlider(g, &cross_seed_slider, getLocalBounds());
    layout.drawTextRightToSlider(g, &interp_switch, getLocalBounds());
}

void Header::resized() { layout.placeUIs(&cross_seed_slider, &interp_switch, getLocalBounds()); }
