#pragma once

#include "PluginProcessor.h"

class MyLookAndFeel : public juce::LookAndFeel_V4 {
public:
    MyLookAndFeel() {
        setColour(juce::Slider::thumbColourId, juce::Colours::red);
    }
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                          const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override {
        auto radius = (float)juce::jmin(width / 2, height / 2) - 5.0f;
        auto centreX = (float)x + (float)width * 0.5f;
        auto centreY = (float)y + (float)height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        g.setColour(juce::Colours::red);
        g.fillEllipse(rx, ry, rw, rw);

        g.setColour(juce::Colours::black);
        // g.drawEllipse(rx, ry, rw, rw, 1.0f);
        //	g.drawRect(x,y,width,height);

        juce::Path p;
        auto pointerLength = radius * 0.33f;
        auto pointerThickness = 2.0f;
        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

        g.setColour(juce::Colours::yellow);
        g.fillPath(p);
    }
    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                          bool hover, bool is_down) override {
        if (button.getToggleState())
            g.setColour(juce::Colours::yellowgreen);
        else
            g.setColour(juce::Colours::grey);
        auto size = std::min(button.getWidth(), button.getHeight());
        auto x = button.getX();
        auto y = button.getY();
	auto padding = 5;
        g.fillEllipse(5, 5, size-10, size-10);
    }
};
