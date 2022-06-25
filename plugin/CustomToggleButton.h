#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>
// #include "MyLookAndFeel.h"

class CustomToggleButton : public juce::ToggleButton
{
public:
    enum class Style
    {
        circle,
        roundedRect,
        rectSwitch
    };
    CustomToggleButton(const CustomToggleButton::Style style) : style{style} {};
    CustomToggleButton(const CustomToggleButton::Style style, const juce::String &name)
        : ToggleButton(name), style{style} {};
    const CustomToggleButton::Style getStyle() { return style; }

private:
    const CustomToggleButton::Style style;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomToggleButton)
};
