#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

class CustomToggleButton : public juce::ToggleButton
{
public:
    enum class Style
    {
        circle,
        roundedRect,
        rectSwitch
    };
    CustomToggleButton(const CustomToggleButton::Style style) : customStyle{style} {};
    CustomToggleButton(const CustomToggleButton::Style style, const juce::String &name)
        : ToggleButton(name), customStyle{style} {};
    const CustomToggleButton::Style getStyle() { return customStyle; }

private:
    const CustomToggleButton::Style customStyle;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomToggleButton)
};
