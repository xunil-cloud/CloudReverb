#include "CustomToggleButton.h"
#include <cmath>

CustomToggleButton::CustomToggleButton(const CustomToggleButton::Style style)
    : customStyle{style}
{
}

CustomToggleButton::CustomToggleButton(const CustomToggleButton::Style style, const juce::String &name)
    : ToggleButton(name), customStyle{style}
{
}

float CustomToggleButton::getAnimatedToggleValue()
{
    const auto desired = getToggleState() ? 1.0f : 0.0f;

    if (!animationInitialised)
    {
        animatedToggleValue = desired;
        targetToggleValue = desired;
        animationInitialised = true;
        return animatedToggleValue;
    }

    if (desired != targetToggleValue)
    {
        targetToggleValue = desired;
        startTimerHz(60);
    }

    return animatedToggleValue;
}

void CustomToggleButton::timerCallback()
{
    const auto delta = targetToggleValue - animatedToggleValue;

    if (std::abs(delta) < 0.01f)
    {
        animatedToggleValue = targetToggleValue;
        stopTimer();
    }
    else
    {
        animatedToggleValue += delta * 0.24f;
    }

    repaint();
    if (auto *parent = getParentComponent())
        parent->repaint();
}
