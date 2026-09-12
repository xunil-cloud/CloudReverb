#include "Mixer.h"

Mixer::Mixer(const juce::String &name, const juce::AudioProcessorValueTreeState &state)
    : Block(name)
{
    using type = ReverbSlider::Type;
    addParameter("dry", state.getParameter("DryOut"), type::Line);
    addParameter("predelay", state.getParameter("PredelayOut"), type::Line);
    addParameter("early", state.getParameter("EarlyOut"), type::Line);
    addParameter("late", state.getParameter("MainOut"), type::Line);
    addParameter("width", state.getParameter("Width"), type::Line);
}

void Mixer::paint(juce::Graphics &g)
{
    Block::paint(g);
    layout.drawTitle(g, getName(), getLocalBounds());
    layout.drawTextUnderSlider(g, sliders[0].get(), getLocalBounds());
    layout.drawTextUnderSlider(g, sliders[1].get(), getLocalBounds());
    layout.drawTextUnderSlider(g, sliders[2].get(), getLocalBounds());
    layout.drawTextUnderSlider(g, sliders[3].get(), getLocalBounds());
    layout.drawTextUnderSlider(g, sliders[4].get(), getLocalBounds());
}
void Mixer::resized()
{
    layout.placeUIs(sliders[0].get(), sliders[1].get(), sliders[2].get(), sliders[3].get(),
                    sliders[4].get(), getLocalBounds());
}
