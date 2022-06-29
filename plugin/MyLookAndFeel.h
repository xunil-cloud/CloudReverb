#pragma once

#include "CustomToggleButton.h"
#include "BinaryData.h"

class MyLookAndFeel : public juce::LookAndFeel_V4
{
    /*
    windowBackground    #2b2b2b #323e44
    widgetBackground    #484848 #263238
    menuBackground      #2b2b2b #323e44
    outline             #121212 #8e989b
    defaultText         #d6d6d6 #ffffff
    defaultFill         #225ebd #42a2c8
    highlightedText     #fff1ff #ffffff
    highlightedFill     #227dbd #181f22
    menuText            #ffffff #ffffff
    numColours
*/
public:
    MyLookAndFeel();

    void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos,
                          const float rotaryStartAngle, const float rotaryEndAngle,
                          juce::Slider &) override;
    void drawToggleButton(juce::Graphics &g, juce::ToggleButton &button, bool hover,
                          bool is_down) override;
    juce::Font getLabelFont(juce::Label &label) override
    {
        auto fontSize = label.getHeight() / 40.f * 38;
        return getCustomFont().withHeight(fontSize);
    }
    juce::Font getTextButtonFont(juce::TextButton &, int buttonHeight) override
    {
        auto fontSize = buttonHeight / 40.f * 38;
        return getCustomFont().withHeight(fontSize);
    }
    juce::Label *createSliderTextBox(juce::Slider &slider) override
    {
        auto *l = LookAndFeel_V4::createSliderTextBox(slider);

        if (slider.getSliderStyle() == juce::Slider::LinearBar ||
            slider.getSliderStyle() == juce::Slider::LinearBarVertical)
        {
            l->setColour(juce::Label::ColourIds::backgroundColourId, juce::Colour(0xff484848));
        }

        return l;
    }
    void drawLabel(juce::Graphics &g, juce::Label &label) override;
    void fillTextEditorBackground(juce::Graphics &g, int width, int height,
                                  juce::TextEditor &textEditor) override;
    void drawTextEditorOutline(juce::Graphics &g, int width, int height,
                               juce::TextEditor &textEditor) override;
    // juce::Font getSliderPopupFont(juce::Slider &) override
    // {
    //     return juce::Font("Hack", 18, juce::Font::plain);
    // }
    static const juce::Font getCustomFont()
    {
        static auto typeface = juce::Typeface::createSystemTypefaceFor(
            BinaryData::PragatiNarrowRegular_ttf, BinaryData::PragatiNarrowRegular_ttfSize);
        return juce::Font(typeface);
    }
};
