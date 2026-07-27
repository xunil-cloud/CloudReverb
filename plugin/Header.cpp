#include "Header.h"

Header::Header(const juce::String &name, const juce::AudioProcessorValueTreeState &state)
    : Block(name), cross_seed_slider("cross seed", ReverbSlider::Type::Circle)
{
    setInterceptsMouseClicks(false, true);

    interp_switch.setName("interpolation");
    auto param = state.getParameter("Interpolation");
    attachment_interp_switch = std::make_unique<juce::ButtonParameterAttachment>(
        *param, interp_switch);

    param = state.getParameter("CrossSeed");
    attachment_cross_seed = std::make_unique<juce::SliderParameterAttachment>(
        *param, cross_seed_slider);

    cross_seed_slider.setPopupDisplayEnabled(true, true, this);

    combobox.addItem("init", commandInitPreset);
    combobox.addItem("save", commandSavePreset);
    combobox.addItem("load", commandLoadPreset);
    combobox.addSeparator();
    combobox.addItem("Chorus Delay", 1);
    combobox.addItem("Dull Echoes", 2);
    combobox.addItem("Hyperplane", 3);
    combobox.addItem("Medium Space", 4);
    combobox.addItem("Noise in the Hallway", 5);
    combobox.addItem("Rubi-Ka Fields", 6);
    combobox.addItem("Small Room", 7);
    combobox.addItem("The 90s Are Back", 8);
    combobox.addItem("Through the Looking Glass", 9);
    combobox.addItem("Airy Clear Space", 10);
    combobox.addItem("Tiny Booth", 11);
    combobox.addItem("Small Studio", 12);
    combobox.addItem("Warm Room", 13);
    combobox.addItem("Bright Room", 14);
    combobox.addItem("Short Plate", 15);
    combobox.addItem("Drum Chamber", 16);
    combobox.addItem("Vocal Chamber", 17);
    combobox.addItem("Medium Hall Clear", 18);
    combobox.addItem("Wide Hall", 19);
    combobox.addItem("Deep Hall", 20);
    combobox.addItem("Cathedral Air", 21);
    combobox.addItem("Cinematic Bloom", 22);
    combobox.addItem("Infinite Chamber", 23);
    combobox.addItem("Galactic Bloom", 24);
    combobox.addItem("Nebula Drift", 25);
    combobox.addItem("Cosmic Shimmer", 26);
    combobox.addItem("Frozen Orbit", 27);
    combobox.addItem("Black Hole Tail", 28);
    combobox.addItem("Starlight Plate", 29);
    combobox.addItem("Event Horizon", 30);

    addAndMakeVisible(interp_switch);
    addAndMakeVisible(cross_seed_slider);
    addAndMakeVisible(combobox);
    setOpaque(false);
}
void Header::paint(juce::Graphics &g)
{
    layout.drawTitle(g, getName(), getLocalBounds());
    layout.drawTextRightToSlider(g, &cross_seed_slider, getLocalBounds());
    layout.drawTextRightToSlider(g, &interp_switch, getLocalBounds());
    layout.drawPresetTitle(g, &combobox, getLocalBounds());
}

void Header::resized()
{
    layout.placeUIs(&cross_seed_slider, &interp_switch, &combobox, getLocalBounds());
}
