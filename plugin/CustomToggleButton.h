#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

class CustomToggleButton : public juce::ToggleButton, private juce::Timer
{
public:
    enum class Style
    {
        circle,
        roundedRect,
        rectSwitch
    };

    CustomToggleButton(const CustomToggleButton::Style style);
    CustomToggleButton(const CustomToggleButton::Style style, const juce::String &name);

    const CustomToggleButton::Style getStyle() const { return customStyle; }
    float getAnimatedToggleValue();

private:
    void timerCallback() override;

    const CustomToggleButton::Style customStyle;
    float animatedToggleValue{0.0f};
    float targetToggleValue{0.0f};
    bool animationInitialised{false};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomToggleButton)
};
