#include "ReverbSlider.h"

ReverbSlider::ReverbSlider(const juce::String& name, const ReverbSlider::Type type) :juce::Slider(name), type(type) {
    setName(name);
    setLookAndFeel(&myLookAndFeel);

    switch (type) {
        case Type::Circle:
            setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
            break;
        case Type::Line:
            setSliderStyle(juce::Slider::LinearVertical);
            break;
    }

    setMouseDragSensitivity(300);
    setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    //setPopupDisplayEnabled(true, true, getParentComponent());
    setNumDecimalPlacesToDisplay(2);
}
