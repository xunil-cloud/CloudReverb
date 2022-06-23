#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "Layout.h"

class ThreePartLayout : public Layout
{
public:
    void placeUIs(juce::Slider *numbox_1, juce::Slider *numbox_2, juce::Slider *slider_1,
                  juce::Slider *slider_2, juce::Slider *mod_amt, juce::Slider *mod_rate,
                  juce::Button *button, const juce::Rectangle<int> &bound);
    // void drawTextUnderSlider(juce::Graphics &g, ReverbSlider *slider,
    //                          const juce::Rectangle<int> &bound);
    // void drawTextRightToSlider(juce::Graphics &g, ReverbSlider *slider,
    //                            const juce::Rectangle<int> &bound);
    // void drawTitle(juce::Graphics &g, const ::juce::String &, const juce::Rectangle<int> &bound);

    // protected:
    //     const float WIDTH{540};
    //     const float HEIGHT{220};
    //     const float X_PADDING{20};
    //     const float Y_PADDING{15};
};
