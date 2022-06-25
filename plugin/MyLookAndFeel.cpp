#include "MyLookAndFeel.h"

MyLookAndFeel::MyLookAndFeel()
    : juce::LookAndFeel_V4::LookAndFeel_V4({0xff2b2b2b, 0xff484848, 0xff2b2b2b, 0xff121212,
                                            0xffd6d6d6, 0xff225ebd, 0xfffff1ff, 0xff227dbd,
                                            0xffffffff})
{
    setColour(juce::Slider::trackColourId, juce::Colour(0xff225ebd));
    setColour(juce::Slider::thumbColourId, juce::Colour(0xff225ebd));

    setColour(juce::Label::backgroundColourId, juce::Colour(0xff484848));
    setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour(0xff383838));
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
