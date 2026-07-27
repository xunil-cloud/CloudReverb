#pragma once

#include "CustomToggleButton.h"
#include "BinaryData.h"

class MyLookAndFeel : public juce::LookAndFeel_V4
{
public:
    static inline bool isLightMode{false};
    void setLightMode(bool light);

    MyLookAndFeel();

    void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos,
                          const float rotaryStartAngle, const float rotaryEndAngle,
                          juce::Slider &) override;
    void drawLinearSlider(juce::Graphics &g, int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          const juce::Slider::SliderStyle style, juce::Slider &slider) override;
    void drawToggleButton(juce::Graphics &g, juce::ToggleButton &button, bool hover,
                          bool is_down) override;
                          
    juce::Font getLabelFont(juce::Label &label) override
    {
        auto fontSize = label.getHeight() / 40.f * 24.f;
        return getCustomFont().withHeight(fontSize);
    }
    
    juce::Font getTextButtonFont(juce::TextButton &, int buttonHeight) override
    {
        auto fontSize = buttonHeight / 40.f * 24.f;
        return getCustomFont().withHeight(fontSize);
    }
    
    juce::Label *createSliderTextBox(juce::Slider &slider) override
    {
        auto *l = LookAndFeel_V4::createSliderTextBox(slider);

        if (slider.getSliderStyle() == juce::Slider::LinearBar ||
            slider.getSliderStyle() == juce::Slider::LinearBarVertical)
        {
            if (isLightMode)
            {
                l->setColour(juce::Label::ColourIds::backgroundColourId, juce::Colour(0xff9db6cf));
                l->setColour(juce::Label::ColourIds::textColourId, juce::Colour(0xff0b1017));
                l->setColour(juce::Label::ColourIds::outlineColourId, juce::Colour(0xff7f9ab5));
                l->setColour(juce::Label::ColourIds::backgroundWhenEditingColourId, juce::Colour(0xff8faac5));
                l->setColour(juce::Label::ColourIds::textWhenEditingColourId, juce::Colour(0xff0b1017));
            }
            else
            {
                l->setColour(juce::Label::ColourIds::backgroundColourId, juce::Colour(0xff0b1017));
                l->setColour(juce::Label::ColourIds::textColourId, juce::Colour(0xffe0e6ed));
                l->setColour(juce::Label::ColourIds::outlineColourId, juce::Colour(0xff28374a));
                l->setColour(juce::Label::ColourIds::backgroundWhenEditingColourId, juce::Colour(0xff1c2738));
                l->setColour(juce::Label::ColourIds::textWhenEditingColourId, juce::Colour(0xffe0e6ed));
            }
        }

        return l;
    }
    
    void drawComboBox(juce::Graphics &g, int width, int height, bool isButtonDown,
                      int buttonX, int buttonY, int buttonW, int buttonH,
                      juce::ComboBox &box) override;

    // Declared for the existing implementation in MyLookAndFeel.cpp.
    // Intentionally not marked override to remain compatible with JUCE versions
    // where this LookAndFeel hook has a different declaration or is unavailable.
    juce::PopupMenu::Options getOptionsForComboBoxPopupMenu(juce::ComboBox &box,
                                                            juce::Label &label);

    void drawLabel(juce::Graphics &g, juce::Label &label) override;
    void fillTextEditorBackground(juce::Graphics &g, int width, int height,
                                  juce::TextEditor &textEditor) override;
    void drawTextEditorOutline(juce::Graphics &g, int width, int height,
                               juce::TextEditor &textEditor) override;

    static const juce::Font getTitleFont()
    {
        if (auto typeface = juce::Typeface::createSystemTypefaceFor(
                BinaryData::ZenDotsRegular_ttf, BinaryData::ZenDotsRegular_ttfSize))
        {
            return juce::Font(juce::FontOptions(typeface));
        }
        return juce::Font(juce::FontOptions("Arial", 16.0f, juce::Font::bold));
    }

    static const juce::Font getCustomFont()
    {
        if (auto typeface = juce::Typeface::createSystemTypefaceFor(
                BinaryData::ShareTechRegular_ttf, BinaryData::ShareTechRegular_ttfSize))
        {
            return juce::Font(juce::FontOptions(typeface));
        }
        return juce::Font(juce::FontOptions("Arial", 16.0f, juce::Font::plain));
    }
};
