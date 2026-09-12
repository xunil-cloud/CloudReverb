#include "ReverbSlider.h"

ReverbSlider::ReverbSlider(const juce::String &name, const ReverbSlider::Type type)
    : juce::Slider(name), type(type)
{
    setName(name);

    switch (type)
    {
    case Type::Circle:
        setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        break;
    case Type::Line:
        setSliderStyle(juce::Slider::LinearVertical);
        setSliderSnapsToMousePosition(false);
        break;
    }

    setMouseDragSensitivity(normalDragSensitivity);
    setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    setNumDecimalPlacesToDisplay(2);
}

void ReverbSlider::updateDragSensitivity(const juce::ModifierKeys &mods)
{
    // Shift-drag = fine adjustment / precision mode.
    // Higher sensitivity value in JUCE means a longer mouse movement is needed
    // for the same parameter change, so the control feels more precise.
    setMouseDragSensitivity(mods.isShiftDown() ? fineDragSensitivity : normalDragSensitivity);
}

void ReverbSlider::mouseDown(const juce::MouseEvent &event)
{
    updateDragSensitivity(event.mods);

    lineFineDragActive = type == Type::Line && event.mods.isShiftDown();
    if (lineFineDragActive)
    {
        lineFineStartValue = getValue();
        lineFineStartY = event.getScreenPosition().getY();
    }

    juce::Slider::mouseDown(event);

    // Re-read after JUCE initialises the drag, so Shift+drag starts from the
    // exact value under the current fader thumb without a jump.
    if (lineFineDragActive)
    {
        lineFineStartValue = getValue();
        lineFineStartY = event.getScreenPosition().getY();
    }
}

void ReverbSlider::mouseDrag(const juce::MouseEvent &event)
{
    updateDragSensitivity(event.mods);

    if (type == Type::Line && event.mods.isShiftDown())
    {
        if (!lineFineDragActive)
        {
            lineFineDragActive = true;
            lineFineStartValue = getValue();
            lineFineStartY = event.getScreenPosition().getY();
        }

        const auto range = getMaximum() - getMinimum();
        const auto pixelsForFullRange = juce::jmax(300.0, (double)getHeight() * 8.0);
        const auto deltaPixels = (double)(lineFineStartY - event.getScreenPosition().getY());
        const auto newValue = juce::jlimit(getMinimum(), getMaximum(),
                                           lineFineStartValue + deltaPixels / pixelsForFullRange * range);
        setValue(newValue, juce::sendNotificationSync);
        return;
    }

    lineFineDragActive = false;
    juce::Slider::mouseDrag(event);
}

void ReverbSlider::mouseUp(const juce::MouseEvent &event)
{
    lineFineDragActive = false;
    juce::Slider::mouseUp(event);
    setMouseDragSensitivity(normalDragSensitivity);
}
