#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include "MyLookAndFeel.h"

class ReverbSlider : public juce::Slider
{
public:
    enum class Type
    {
        Circle,
        Line
    };
    ReverbSlider(const juce::String &name, const ReverbSlider::Type type);

private:
    juce::String name;
    ReverbSlider::Type type;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbSlider)
};
