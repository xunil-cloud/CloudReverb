#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

class ReverbSlider : public juce::Slider
{
public:
    enum class Type
    {
        Circle,
        Line
    };
    ReverbSlider(const juce::String &name, const ReverbSlider::Type type);

    void mouseDown(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &event) override;
    void mouseUp(const juce::MouseEvent &event) override;

private:
    void updateDragSensitivity(const juce::ModifierKeys &mods);

    static constexpr int normalDragSensitivity{300};
    static constexpr int fineDragSensitivity{1800};

    bool lineFineDragActive{false};
    double lineFineStartValue{0.0};
    int lineFineStartY{0};

    juce::String name;
    ReverbSlider::Type type;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbSlider)
};
