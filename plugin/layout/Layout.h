#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../ReverbSlider.h"
class Layout
{
public:
    Layout(float originalWidth = 540, float originalHeight = 220)
        : WIDTH{originalWidth}, HEIGHT{originalHeight}
    {
    }
    void drawTextUnderSlider(juce::Graphics &g, ReverbSlider *slider,
                             const juce::Rectangle<int> &bound, int gap = 15);
    void drawTextRightToSlider(juce::Graphics &g, juce::Component *slider,
                               const juce::Rectangle<int> &bound);
    virtual void drawTitle(juce::Graphics &g, const ::juce::String &,
                           const juce::Rectangle<int> &bound);

    void setBound(const juce::Rectangle<int> &newBound) { this->bound = newBound; }

    const int computeRatio()
    {
        const float ratio = bound.getWidth() / WIDTH > bound.getHeight() / HEIGHT
                                ? bound.getHeight() / HEIGHT
                                : bound.getWidth() / WIDTH;
        return ratio;
    }

protected:
    juce::Rectangle<int> bound;
    float WIDTH;
    float HEIGHT;
    const float X_PADDING{20};
    const float Y_PADDING{15};
};

class TapLayout : public Layout
{
public:
    void placeUIs(juce::Slider *numbox_1, juce::Slider *numbox_2, juce::Slider *slider_1,
                  juce::Slider *slider_2, juce::Slider *slider_3,
                  const juce::Rectangle<int> &bound);
};
class InputLayout : public Layout
{
public:
    void placeUIs(juce::Slider *slider_1, juce::Slider *slider_2, juce::Slider *slider_3,
                  juce::Slider *slider_4, juce::Button *button_3, juce::Button *button_4,
                  const juce::Rectangle<int> &bound);
};
class MixerLayout : public Layout
{
public:
    MixerLayout() : Layout(330, 220) {}
    void placeUIs(juce::Slider *slider_1, juce::Slider *slider_2, juce::Slider *slider_3,
                  juce::Slider *slider_4, const juce::Rectangle<int> &bound);
};
class EqLayout : public Layout
{
public:
    EqLayout() : Layout(330, 450) {}
    void placeUIs(juce::Slider *low_1, juce::Slider *low_2, juce::Slider *high_1,
                  juce::Slider *high_2, juce::Slider *cutoff, juce::Button *low_button,
                  juce::Button *high_button, juce::Button *cutoff_button,
                  const juce::Rectangle<int> &bound);
};
class RouteBlock;
class RouteLayout : public Layout
{
public:
    void placeUIs(juce::Drawable *diagram, juce::Slider *slider, juce::Button *button,
                  const juce::Rectangle<int> &bound);
    void drawTextOndiagram(juce::Graphics &g, const juce::String &fist_block,
                           const juce::String &second_block, const juce::Rectangle<int> &bound);
};
class HeaderLayout : public Layout
{
public:
    HeaderLayout() : Layout(1418, 110) {}
    void drawTitle(juce::Graphics &g, const ::juce::String &,
                   const juce::Rectangle<int> &bound) override;
    void placeUIs(juce::Slider *cross_seed, juce::Button *interpolation,
                  const juce::Rectangle<int> &bound);
};
