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

    combobox.addItem("Chorus Delay", 1);
    combobox.addItem("Dull Echoes", 2);
    combobox.addItem("Hyperplane", 3);
    combobox.addItem("Mudium Space", 4);
    combobox.addItem("Noise in the Hallway", 5);
    combobox.addItem("Rubi-Ka Fields", 6);
    combobox.addItem("Small Room", 7);
    combobox.addItem("The 90s Are Back", 8);
    combobox.addItem("Through the Looking Glass", 9);

    addAndMakeVisible(interp_switch);
    addAndMakeVisible(cross_seed_slider);
    addAndMakeVisible(combobox);
}
void Header::paint(juce::Graphics &g)
{
    layout.drawTitle(g, getName(), getLocalBounds());
    layout.drawTextRightToSlider(g, &cross_seed_slider, getLocalBounds());
    layout.drawTextRightToSlider(g, &interp_switch, getLocalBounds());
}

void Header::resized()
{
    layout.placeUIs(&cross_seed_slider, &interp_switch, &combobox, getLocalBounds());
}
