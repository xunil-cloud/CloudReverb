#pragma once

#include "PluginProcessor.h"

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
                          juce::Slider &) override
    {
        auto radius = (float)juce::jmin(width / 2, height / 2) - 5.0f;
        auto centreX = (float)x + (float)width * 0.5f;
        auto centreY = (float)y + (float)height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        g.setColour(findColour(juce::Slider::thumbColourId));
        // g.setColour(juce::Colour(0xff225ebd));
        g.fillEllipse(rx, ry, rw, rw);

        g.setColour(juce::Colour(0xff121212));
        // g.drawEllipse(rx, ry, rw, rw, 1.5f);
        // g.drawRect(x,y,width,height);

        juce::Path p;
        auto pointerLength = radius * 0.33f;
        auto pointerThickness = 2.0f;
        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

        g.setColour(juce::Colours::black);
        // g.setColour((getCurrentColourScheme().getUIColour(juce::LookAndFeel_V4::ColourScheme::UIColour::outline)));
        g.fillPath(p);
    }
    void drawToggleButton(juce::Graphics &g, juce::ToggleButton &button, bool hover,
                          bool is_down) override
    {
        if (button.getToggleState())
            // g.setColour(juce::Colour(66, 200, 158));
            g.setColour(juce::Colour(0xffbd8122));
        else
            g.setColour(juce::Colours::grey);
        auto size = std::min(button.getWidth(), button.getHeight());
        auto x = button.getX();
        auto y = button.getY();
        auto padding = 1;

        g.fillEllipse(padding, padding, size - padding * 2, size - padding * 2);
        if (hover)
        {
            g.setColour(juce::Colours::white);
            g.drawEllipse(padding, padding, size - padding * 2, size - padding * 2, 1);
        }
        // g.fillRoundedRectangle(padding, padding, size - padding * 2, size - padding * 2, size *
        // 0.2);
    }
    juce::Font getLabelFont(juce::Label &label) override
    {
        auto fontSize = label.getHeight() / 40.f * 38;
        return juce::Font("Pragati Narrow", fontSize, juce::Font::plain);
    }
    juce::Font getTextButtonFont(juce::TextButton &, int buttonHeight) override
    {
        auto fontSize = buttonHeight / 40.f * 38;
        return juce::Font("Pragati Narrow", fontSize, juce::Font::plain);
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
};
