#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "Layout.h"

class ThreePartLayout : public Layout
{
public:
    void placeUIs(juce::Slider *numbox_1, juce::Slider *numbox_2, juce::Slider *slider_1,
                  juce::Slider *slider_2, juce::Slider *mod_amt, juce::Slider *mod_rate,
                  juce::Button *button, const juce::Rectangle<int> &bound);
};
