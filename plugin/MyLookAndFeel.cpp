#include "MyLookAndFeel.h"
#include "BinaryData.h"

MyLookAndFeel::MyLookAndFeel()
    : juce::LookAndFeel_V4::LookAndFeel_V4({0xff2b2b2b, 0xff484848, 0xff2b2b2b, 0xff121212,
                                            0xffd6d6d6, 0xff225ebd, 0xfffff1ff, 0xff227dbd,
                                            0xffffffff})
{

    auto futuraMediumFont = juce::Typeface::createSystemTypefaceFor(
        BinaryData::PragatiNarrowRegular_ttf, BinaryData::PragatiNarrowRegular_ttfSize);
    // juce::LookAndFeel::setDefaultLookAndFeel(this);
    // setDefaultSansSerifTypeface(futuraMediumFont);
    setColour(juce::Slider::trackColourId, juce::Colour(0xff2660bd));
    setColour(juce::Slider::thumbColourId, juce::Colour(0xff2660bd));

    setColour(juce::Label::backgroundColourId, juce::Colour(0xff484848));
    setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour(0xff383838));
    // setDefaultSansSerifTypefaceName("Roboto");
}
void MyLookAndFeel::drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height,
                                     float sliderPos, const float rotaryStartAngle,
                                     const float rotaryEndAngle, juce::Slider &)
{
    auto radius = (float)juce::jmin(width / 2, height / 2) - 5.0f;
    auto centreX = (float)x + (float)width * 0.5f;
    auto centreY = (float)y + (float)height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    g.setColour(findColour(juce::Slider::thumbColourId));
    g.fillEllipse(rx, ry, rw, rw);

    juce::Path p;
    auto pointerLength = radius * 0.65;
    auto pointerThickness = 3.5 * width / 65.f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

    g.setColour(juce::Colour(0xff343434));
    g.fillPath(p);
}
void MyLookAndFeel::drawToggleButton(juce::Graphics &g, juce::ToggleButton &button, bool hover,
                                     bool is_down)
{

    auto *customButton = dynamic_cast<CustomToggleButton *>(&button);
    if (customButton != nullptr)
    {
        auto padding = 1;
        juce::Colour buttonOnColour{0xff269cbd};
        juce::Colour buttonOffColour{0xff555555};

        using style = CustomToggleButton::Style;
        auto baseColour = button.getToggleState() ? buttonOnColour : buttonOffColour;

        if (is_down || hover)
        {
            if (button.getToggleState())
                baseColour = baseColour.contrasting(is_down ? 0.2f : 0.08f);
            else
                baseColour = baseColour.contrasting(is_down ? 0.02f : 0.08);
        }
        g.setColour(baseColour);
        switch (customButton->getStyle())
        {
        case style::circle:
            g.fillEllipse(button.getLocalBounds().toFloat());
            break;

        case style::roundedRect:
            g.fillRoundedRectangle(button.getLocalBounds().toFloat(), 3.f);
            break;

        case style::rectSwitch:
            if (is_down || hover)
            {
                buttonOffColour = buttonOffColour.contrasting(is_down ? 0.02f : 0.08f);
            }

            g.setColour(buttonOffColour);
            g.fillRoundedRectangle(padding, padding, button.getWidth() - padding * 2,
                                   button.getHeight() - padding * 2, 3.f);
            g.setColour(buttonOnColour);
            if (button.getToggleState())
                g.fillRoundedRectangle(button.getWidth() / 2.f, 0, button.getWidth() / 2.f,
                                       button.getHeight(), 3.f);
            else
                g.fillRoundedRectangle(0, 0, button.getWidth() / 2.f, button.getHeight(), 3.f);

            break;
        default:
            juce::LookAndFeel_V4::drawToggleButton(g, button, hover, is_down);
            break;
        }
    }
    else
    {
        juce::LookAndFeel_V4::drawToggleButton(g, button, hover, is_down);
    }
}
void MyLookAndFeel::drawLabel(juce::Graphics &g, juce::Label &label)
{
    // g.fillAll(label.findColour(juce::Label::backgroundColourId));
    g.setColour(label.findColour(juce::Label::backgroundColourId));
    g.fillRoundedRectangle(label.getLocalBounds().toFloat(), 3.f);

    if (!label.isBeingEdited())
    {
        auto alpha = label.isEnabled() ? 1.0f : 0.5f;
        const juce::Font font(getLabelFont(label));

        g.setColour(label.findColour(juce::Label::textColourId).withMultipliedAlpha(alpha));
        g.setFont(font);

        auto textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());

        g.drawFittedText(label.getText(), textArea, label.getJustificationType(),
                         juce::jmax(1, (int)((float)textArea.getHeight() / font.getHeight())),
                         label.getMinimumHorizontalScale());

        g.setColour(label.findColour(juce::Label::outlineColourId).withMultipliedAlpha(alpha));
    }
    else if (label.isEnabled())
    {
        g.setColour(label.findColour(juce::Label::outlineColourId));
    }

    g.drawRoundedRectangle(label.getLocalBounds().toFloat(), 3.f, 1.f);
}
void MyLookAndFeel::fillTextEditorBackground(juce::Graphics &g, int width, int height,
                                             juce::TextEditor &textEditor)
{
    if (dynamic_cast<juce::AlertWindow *>(textEditor.getParentComponent()) != nullptr)
    {
        g.setColour(textEditor.findColour(juce::TextEditor::backgroundColourId));
        g.fillRect(0, 0, width, height);

        g.setColour(textEditor.findColour(juce::TextEditor::outlineColourId));
        g.drawHorizontalLine(height - 1, 0.0f, static_cast<float>(width));
    }
    else
    {
        g.setColour(textEditor.findColour(juce::TextEditor::backgroundColourId));
        g.fillRoundedRectangle(0, 0, width, height, 3.f);
    }
}

void MyLookAndFeel::drawTextEditorOutline(juce::Graphics &g, int width, int height,
                                          juce::TextEditor &textEditor)
{
}
