#ifdef _MSC_VER
#pragma warning(disable: 4702 4244 4305 4458 4100)
#endif

#include "MyLookAndFeel.h"

MyLookAndFeel::MyLookAndFeel()
    : juce::LookAndFeel_V4::LookAndFeel_V4({0xff0a0f18, 0xff1c2738, 0xff0a0f18, 0xff2e3e54,
                                            0xffcfd6d9, 0xff00bcd4, 0xffffffff, 0xff26a69a,
                                            0xffffffff})
{
    setLightMode(false);
}

void MyLookAndFeel::setLightMode(bool light)
{
    isLightMode = light;
    if (isLightMode)
    {
        setColour(juce::Slider::trackColourId, juce::Colour(0xff8797ad));
        setColour(juce::Slider::thumbColourId, juce::Colour(0xff0097a7));
        setColour(juce::Label::backgroundColourId, juce::Colour(0xffcbd6e3));
        setColour(juce::Label::textColourId, juce::Colour(0xff142033));
        setColour(juce::Label::outlineColourId, juce::Colour(0xff74869e));
        setColour(juce::PopupMenu::backgroundColourId, juce::Colour(0xffdbe3ec));
        setColour(juce::PopupMenu::textColourId, juce::Colour(0xff0b1017));
        setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour(0xffbfd0e2));
        setColour(juce::PopupMenu::highlightedTextColourId, juce::Colour(0xff0b223a));
        setColour(juce::ResizableWindow::backgroundColourId, juce::Colour(0xffe2edf7));
        setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xffd8e0ea));
        setColour(juce::ComboBox::textColourId, juce::Colour(0xff0b1017));
        setColour(juce::ComboBox::outlineColourId, juce::Colour(0xff8194aa));
        setColour(juce::ComboBox::arrowColourId, juce::Colour(0xff334a63));
        setColour(juce::TextEditor::backgroundColourId, juce::Colour(0xff9db6cf));
        setColour(juce::TextEditor::textColourId, juce::Colour(0xff0b1017));
        setColour(juce::TextEditor::outlineColourId, juce::Colour(0xff7f9ab5));
        setColour(juce::TextEditor::highlightColourId, juce::Colour(0xff9ecbd4));
        setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff9db6cf));
        setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xff0b1017));
        setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xff7f9ab5));
        setColour(juce::TextButton::buttonColourId, juce::Colour(0xffc3cedc));
        setColour(juce::TextButton::textColourOffId, juce::Colour(0xff182638));
        setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xff0097a7));
    }
    else
    {
        setColour(juce::Slider::trackColourId, juce::Colour(0xff2e3e54));
        setColour(juce::Slider::thumbColourId, juce::Colour(0xff00bcd4));
        setColour(juce::Label::backgroundColourId, juce::Colour(0xff0b1017));
        setColour(juce::Label::textColourId, juce::Colour(0xffe0e6ed));
        setColour(juce::Label::outlineColourId, juce::Colour(0xff28374a));
        setColour(juce::PopupMenu::backgroundColourId, juce::Colour(0xff0b1017));
        setColour(juce::PopupMenu::textColourId, juce::Colour(0xffe0e6ed));
        setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour(0xff2e3e54));
        setColour(juce::PopupMenu::highlightedTextColourId, juce::Colour(0xffffffff));
        setColour(juce::ResizableWindow::backgroundColourId, juce::Colour(0xff0a0f18));
        setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xff0b1017));
        setColour(juce::ComboBox::textColourId, juce::Colour(0xffe0e6ed));
        setColour(juce::ComboBox::outlineColourId, juce::Colour(0xff28374a));
        setColour(juce::ComboBox::arrowColourId, juce::Colour(0xffe0e6ed));
        setColour(juce::TextEditor::backgroundColourId, juce::Colour(0xff0b1017));
        setColour(juce::TextEditor::textColourId, juce::Colour(0xffe0e6ed));
        setColour(juce::TextEditor::outlineColourId, juce::Colour(0xff28374a));
        setColour(juce::TextEditor::highlightColourId, juce::Colour(0xff005f70));
        setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff0b1017));
        setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xffe0e6ed));
        setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xff28374a));
        setColour(juce::TextButton::buttonColourId, juce::Colour(0xff1c2738));
        setColour(juce::TextButton::textColourOffId, juce::Colour(0xffd6dce4));
        setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xff00bcd4));
    }
}

void MyLookAndFeel::drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height,
                                     float sliderPos, const float rotaryStartAngle,
                                     const float rotaryEndAngle, juce::Slider &)
{
    const auto diameter = (float)juce::jmin(width, height);
    const auto knobScale = juce::jlimit(0.45f, 2.35f, diameter / 65.0f);
    const auto edgePadding = juce::jmax(2.0f, 4.0f * knobScale);
    const auto arcStroke = juce::jlimit(2.0f, 7.0f, 3.5f * knobScale);
    const auto lightBorder = juce::jlimit(1.1f, 4.2f, 2.0f * knobScale);
    const auto darkBorder = juce::jlimit(0.9f, 3.5f, 1.5f * knobScale);
    const auto innerGap = juce::jmax(3.0f, 6.0f * knobScale);
    const auto pointerWidth = juce::jlimit(1.0f, 3.8f, 2.4f * knobScale);
    const auto pointerInset = juce::jmax(1.5f, 2.0f * knobScale);

    auto radius = diameter * 0.5f - edgePadding;
    auto centreX = (float)x + (float)width * 0.5f;
    auto centreY = (float)y + (float)height * 0.5f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // 1. Draw Track Arc
    juce::Path trackArc;
    trackArc.addCentredArc(centreX, centreY, radius, radius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
    g.setColour(isLightMode ? juce::Colour(0xff7f90a7) : juce::Colour(0xff2e3e54));
    g.strokePath(trackArc, juce::PathStrokeType(arcStroke, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // 2. Draw Cyan Value Arc
    if (sliderPos > 0.001f)
    {
        juce::Path valueArc;
        valueArc.addCentredArc(centreX, centreY, radius, radius, 0.0f, rotaryStartAngle, angle, true);
        g.setColour(isLightMode ? juce::Colour(0xff00acc1) : juce::Colour(0xff00bcd4));
        g.strokePath(valueArc, juce::PathStrokeType(arcStroke, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    }

    // 3. Draw Knob Inner Thumb
    auto thumbRadius = juce::jmax(1.0f, radius - innerGap);
    if (isLightMode)
    {
        // Light mode: white fill with stronger border + subtle inner shadow
        g.setGradientFill(juce::ColourGradient(
            juce::Colour(0xffedf2f8), centreX, centreY - thumbRadius * 0.3f,
            juce::Colour(0xffd3ddea), centreX, centreY + thumbRadius,
            false));
        g.fillEllipse(centreX - thumbRadius, centreY - thumbRadius, thumbRadius * 2.0f, thumbRadius * 2.0f);
        g.setColour(juce::Colour(0xff687d97)); // Strong visible border
        g.drawEllipse(centreX - thumbRadius, centreY - thumbRadius, thumbRadius * 2.0f, thumbRadius * 2.0f, lightBorder);
    }
    else
    {
        g.setColour(juce::Colour(0xff16202e));
        g.fillEllipse(centreX - thumbRadius, centreY - thumbRadius, thumbRadius * 2.0f, thumbRadius * 2.0f);
        g.setColour(juce::Colour(0xff3a4d69));
        g.drawEllipse(centreX - thumbRadius, centreY - thumbRadius, thumbRadius * 2.0f, thumbRadius * 2.0f, darkBorder);
    }

    // 4. Draw Needle Pointer
    juce::Path pointer;
    auto pointerLength = juce::jmax(1.0f, thumbRadius - pointerInset);
    pointer.addRectangle(-pointerWidth * 0.5f, -thumbRadius + pointerInset, pointerWidth, pointerLength);
    pointer.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
    g.setColour(isLightMode ? juce::Colour(0xff0097a7) : juce::Colour(0xff00bcd4)); // Deeper cyan for light
    g.fillPath(pointer);
}

void MyLookAndFeel::drawLinearSlider(juce::Graphics &g, int x, int y, int width, int height,
                                     float sliderPos, float minSliderPos, float maxSliderPos,
                                     const juce::Slider::SliderStyle style, juce::Slider &slider)
{
    if (style == juce::Slider::LinearVertical)
    {
        const auto sliderName = slider.getName();
        const bool isMixerFader = sliderName == "dry" || sliderName == "predelay"
                                  || sliderName == "early" || sliderName == "late"
                                  || sliderName == "width";
        const auto faderScale = isMixerFader
                                    ? juce::jlimit(0.28f, 2.20f, juce::jmin((float)height / 87.0f,
                                                                             (float)width / 25.0f))
                                    : 1.0f;
        auto trackWidth = isMixerFader ? juce::jlimit(2.0f, juce::jmax(2.0f, (float)width * 0.55f), 6.0f * faderScale)
                                       : 6.0f;
        auto trackX = x + (width - trackWidth) * 0.5f;
        auto trackY = (float)y;
        auto trackHeight = (float)height;
        const auto trackStroke = isMixerFader ? juce::jlimit(0.65f, 2.1f, 1.2f * faderScale)
                                              : 1.2f;

        g.setColour(isLightMode ? juce::Colour(0xffb7c3d2) : juce::Colour(0xff28374a));
        g.fillRoundedRectangle(trackX, trackY, trackWidth, trackHeight, trackWidth * 0.5f);
        g.setColour(isLightMode ? juce::Colour(0xff7f90a7) : juce::Colour(0xff3c4c64));
        g.drawRoundedRectangle(trackX, trackY, trackWidth, trackHeight, trackWidth * 0.5f, trackStroke);

        auto fillHeight = maxSliderPos - sliderPos;
        g.setColour(isLightMode ? juce::Colour(0xff00acc1) : juce::Colour(0xff00bcd4));
        g.fillRoundedRectangle(trackX, sliderPos, trackWidth, fillHeight, trackWidth * 0.5f);

        const auto thumbSize = isMixerFader ? juce::jlimit(6.0f, 24.0f,
                                                           juce::jmin((float)width * 0.90f,
                                                                      (float)height * 0.22f))
                                           : 18.0f;
        auto thumbWidth = thumbSize;
        auto thumbHeight = thumbSize;
        auto thumbX = x + (width - thumbWidth) * 0.5f;
        auto thumbY = sliderPos - thumbHeight * 0.5f;
        const auto thumbStrokeLight = isMixerFader ? juce::jlimit(0.8f, 3.4f, thumbSize * 0.12f)
                                                   : 2.2f;
        const auto thumbStrokeDark = isMixerFader ? juce::jlimit(0.8f, 3.0f, thumbSize * 0.11f)
                                                  : 2.0f;
        const auto dotSize = isMixerFader ? juce::jlimit(1.8f, 6.2f, thumbSize * 0.28f)
                                          : 6.0f;
        
        if (isLightMode)
        {
            g.setColour(juce::Colour(0xffffffff));
            g.fillEllipse(thumbX, thumbY, thumbWidth, thumbHeight);
            g.setColour(juce::Colour(0xff687d97)); // Strong border for light theme
            g.drawEllipse(thumbX, thumbY, thumbWidth, thumbHeight, thumbStrokeLight);
            // Cyan dot in center
            g.setColour(juce::Colour(0xff00acc1));
            g.fillEllipse(thumbX + thumbWidth * 0.5f - dotSize * 0.5f,
                          thumbY + thumbHeight * 0.5f - dotSize * 0.5f,
                          dotSize, dotSize);
        }
        else
        {
            g.setColour(juce::Colour(0xff121822));
            g.fillEllipse(thumbX, thumbY, thumbWidth, thumbHeight);
            g.setColour(juce::Colour(0xff00bcd4));
            g.drawEllipse(thumbX, thumbY, thumbWidth, thumbHeight, thumbStrokeDark);
            // Cyan dot in center, matching the light theme thumb detail
            g.fillEllipse(thumbX + thumbWidth * 0.5f - dotSize * 0.5f,
                          thumbY + thumbHeight * 0.5f - dotSize * 0.5f,
                          dotSize, dotSize);
        }
    }
    else if (style == juce::Slider::LinearBar || style == juce::Slider::LinearBarVertical)
    {
        // Number box sliders (seed, count fields)
        auto bounds = juce::Rectangle<float>((float)x, (float)y, (float)width, (float)height);
        
        if (isLightMode)
        {
            // Grey non-black number boxes for Seed / Stages / Parallel Lines fields
            g.setColour(juce::Colour(0xff9db6cf));
            g.fillRoundedRectangle(bounds, 4.0f);
            g.setColour(juce::Colour(0xff7f9ab5));
            g.drawRoundedRectangle(bounds, 4.0f, 1.15f);
        }
        else
        {
            g.setColour(juce::Colour(0xff0b1017));
            g.fillRoundedRectangle(bounds, 4.0f);
            g.setColour(juce::Colour(0xff28374a));
            g.drawRoundedRectangle(bounds, 4.0f, 1.5f);
        }
        
        // Draw the text value
        auto font = getCustomFont().withHeight(height * 0.45f);
        g.setFont(font);
        g.setColour(isLightMode ? juce::Colour(0xff142033) : juce::Colour(0xffe0e6ed));
        g.drawFittedText(slider.getTextFromValue(slider.getValue()),
                         bounds.toNearestInt(), juce::Justification::centred, 1);
    }
    else
    {
        juce::LookAndFeel_V4::drawLinearSlider(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
    }
}

void MyLookAndFeel::drawToggleButton(juce::Graphics &g, juce::ToggleButton &button, bool hover,
                                     bool is_down)
{
    auto *customButton = dynamic_cast<CustomToggleButton *>(&button);
    if (customButton != nullptr)
    {
        auto padding = 2.0f;
        const auto animatedToggle = customButton->getAnimatedToggleValue();
        juce::Colour buttonOnColour{isLightMode ? juce::Colour(0xff00acc1) : juce::Colour(0xff00bcd4)};
        juce::Colour buttonOffColour{isLightMode ? juce::Colour(0xff74869e) : juce::Colour(0xff0b1017)};

        using style = CustomToggleButton::Style;
        auto baseColour = buttonOffColour.interpolatedWith(buttonOnColour, animatedToggle);

        if (is_down || hover)
        {
            baseColour = baseColour.contrasting(is_down ? 0.15f : 0.05f);
        }
        g.setColour(baseColour);
        auto bounds = button.getLocalBounds().toFloat().reduced(1.0f);

        switch (customButton->getStyle())
        {
        case style::circle:
        {
            juce::Colour circleOnColour(isLightMode ? 0xff00acc1 : 0xff00bcd4);
            juce::Colour circleOffColour(isLightMode ? 0xffaebccd : 0xff3c4c64);
            g.setColour(circleOffColour.interpolatedWith(circleOnColour, animatedToggle));
            g.fillEllipse(bounds);
            
            g.setColour(isLightMode ? juce::Colour(0xff7a8ea5) : juce::Colour(0xff2e3e54));
            g.drawEllipse(bounds, 1.8f);
            
            // ON indicator: small inner dot
            if (animatedToggle > 0.01f)
            {
                g.setColour(juce::Colour(0xffffffff).withAlpha(0.5f * animatedToggle));
                g.fillEllipse(bounds.reduced(bounds.getWidth() * 0.3f));
            }
            break;
        }

        case style::roundedRect:
        {
            juce::Colour pushOnColour(isLightMode ? 0xff00acc1 : 0xff00bcd4);
            juce::Colour pushOffColour(isLightMode ? 0xffaebccd : 0xff3c4c64);
            g.setColour(pushOffColour.interpolatedWith(pushOnColour, animatedToggle));
            g.fillRoundedRectangle(bounds, 4.f);
            
            g.setColour(isLightMode ? juce::Colour(0xff7a8ea5) : juce::Colour(0xff2e3e54));
            g.drawRoundedRectangle(bounds, 4.f, 1.8f);
            
            // ON indicator: check mark or inner fill
            if (animatedToggle > 0.01f)
            {
                g.setColour(juce::Colour(0xffffffff).withAlpha(0.3f * animatedToggle));
                g.fillRoundedRectangle(bounds.reduced(3.0f), 3.0f);
            }
            break;
        }

        case style::rectSwitch:
        {
            auto pillRadius = bounds.getHeight() * 0.5f;
            g.setColour(isLightMode ? juce::Colour(0xffb3c0d0) : juce::Colour(0xff0b1017));
            g.fillRoundedRectangle(bounds, pillRadius);
            
            g.setColour(isLightMode ? juce::Colour(0xff8a9bb5) : juce::Colour(0xff28374a));
            g.drawRoundedRectangle(bounds, pillRadius, 1.8f);

            auto thumbWidth = bounds.getWidth() * 0.5f - padding * 2.0f;
            auto thumbHeight = bounds.getHeight() - padding * 2.0f;
            auto thumbRadius = thumbHeight * 0.5f;

            const auto leftX = padding + 1.0f;
            const auto rightX = bounds.getWidth() - thumbWidth - padding - 1.0f;
            const auto thumbX = leftX + (rightX - leftX) * animatedToggle;
            const auto offThumb = isLightMode ? juce::Colour(0xff4f6178) : juce::Colour(0xff4c5d73);
            g.setColour(offThumb.interpolatedWith(buttonOnColour, animatedToggle));
            g.fillRoundedRectangle(thumbX, padding + 1.0f, thumbWidth, thumbHeight, thumbRadius);
            break;
        }
        default:
            juce::LookAndFeel_V4::drawToggleButton(g, button, hover, is_down);
            break;
        }
    }
    else
    {
        // Plain TextButton (init, Save, Load)
        auto bounds = button.getLocalBounds().toFloat().reduced(1.0f);
        auto isLight = MyLookAndFeel::isLightMode;
        
        if (isLight)
        {
            g.setColour(is_down ? juce::Colour(0xff008b99) : juce::Colour(0xffc3cedc));
            g.fillRoundedRectangle(bounds, 5.0f);
            g.setColour(juce::Colour(0xff74869e));
            g.drawRoundedRectangle(bounds, 5.0f, 1.5f);
        }
        else
        {
            g.setColour(is_down ? juce::Colour(0xff00bcd4) : juce::Colour(0xff1c2738));
            g.fillRoundedRectangle(bounds, 5.0f);
            g.setColour(juce::Colour(0xff2e3e54));
            g.drawRoundedRectangle(bounds, 5.0f, 1.5f);
        }
        
        auto font = getCustomFont().withHeight(bounds.getHeight() * 0.55f);
        g.setFont(font);
        g.setColour(isLight ? juce::Colour(0xff142033) : juce::Colour(0xffd6dce4));
        g.drawFittedText(button.getButtonText(), bounds.toNearestInt(), juce::Justification::centred, 1);
    }
}

void MyLookAndFeel::drawComboBox(juce::Graphics &g, int width, int height, bool isButtonDown,
                                      int buttonX, int buttonY, int buttonW, int buttonH,
                                      juce::ComboBox &box)
{
    juce::ignoreUnused(isButtonDown, box);

    auto bounds = juce::Rectangle<float>(0.5f, 0.5f, (float)width - 1.0f, (float)height - 1.0f);

    if (isLightMode)
    {
        g.setColour(juce::Colour(0xffd8e0ea));
        g.fillRoundedRectangle(bounds, 4.5f);
        g.setColour(juce::Colour(0xff8194aa));
        g.drawRoundedRectangle(bounds, 4.5f, 1.15f);
    }
    else
    {
        g.setColour(juce::Colour(0xff0b1017));
        g.fillRoundedRectangle(bounds, 4.5f);
        g.setColour(juce::Colour(0xff28374a));
        g.drawRoundedRectangle(bounds, 4.5f, 1.2f);
    }

    const auto arrowColour = isLightMode ? juce::Colour(0xff334a63) : juce::Colour(0xffe0e6ed);
    const auto arrowArea = juce::Rectangle<float>((float)buttonX, (float)buttonY, (float)buttonW, (float)buttonH).reduced(5.0f);
    juce::Path arrow;
    arrow.addTriangle(arrowArea.getX(), arrowArea.getCentreY() - 2.0f,
                      arrowArea.getCentreX(), arrowArea.getCentreY() + 3.0f,
                      arrowArea.getRight(), arrowArea.getCentreY() - 2.0f);
    g.setColour(arrowColour);
    g.fillPath(arrow);
}

juce::PopupMenu::Options MyLookAndFeel::getOptionsForComboBoxPopupMenu(juce::ComboBox &box,
                                                                         juce::Label &label)
{
    auto options = juce::LookAndFeel_V4::getOptionsForComboBoxPopupMenu(box, label)
                       .withTargetComponent(&box)
                       .withTargetScreenArea(box.localAreaToGlobal(
                           juce::Rectangle<int>(0, box.getHeight(), box.getWidth(), 1)))
                       .withMinimumWidth(box.getWidth())
                       .withMaximumNumColumns(1)
                       .withStandardItemHeight(juce::jmax(22, box.getHeight()))
                       .withItemThatMustBeVisible(box.getSelectedId());

    if (auto *topLevel = box.getTopLevelComponent())
        options = options.withParentComponent(topLevel);

    return options;
}

void MyLookAndFeel::drawLabel(juce::Graphics &g, juce::Label &label)
{
    auto bounds = label.getLocalBounds().toFloat();

    if (label.getName() == "ZoomCaption")
    {
        g.setColour(isLightMode ? juce::Colour(0xff3d4f63) : juce::Colour(0xff8a9ba8));
        g.setFont(getCustomFont().withHeight(label.getHeight() * 0.80f));
        g.drawFittedText(label.getText(), label.getLocalBounds(), juce::Justification::centredLeft, 1);
        return;
    }

    if (label.getName() == "UndoRedoCaption")
    {
        g.setColour(isLightMode ? juce::Colour(0xff3d4f63) : juce::Colour(0xff8a9ba8));
        g.setFont(getCustomFont().withHeight(12.5f));
        g.drawFittedText(label.getText(), label.getLocalBounds(), juce::Justification::centred, 1);
        return;
    }

    auto *parent = label.getParentComponent();
    const bool isSliderTextBox = dynamic_cast<juce::Slider *>(parent) != nullptr;
    const bool isComboBoxText = dynamic_cast<juce::ComboBox *>(parent) != nullptr;

    if (isSliderTextBox || isComboBoxText)
    {
        const auto background = isLightMode ? juce::Colour(0xff9db6cf) : juce::Colour(0xff0b1017);
        const auto text = isLightMode ? juce::Colour(0xff0b1017) : juce::Colour(0xffe0e6ed);
        const auto outline = isLightMode ? juce::Colour(0xff7f9ab5) : juce::Colour(0xff28374a);

        if (isSliderTextBox)
        {
            g.setColour(background);
            g.fillRoundedRectangle(bounds, 4.0f);
        }

        if (!label.isBeingEdited())
        {
            const auto alpha = label.isEnabled() ? 1.0f : 0.5f;
            const auto font = getLabelFont(label);
            g.setFont(font);
            g.setColour(text.withMultipliedAlpha(alpha));

            auto textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());
            if (isComboBoxText)
                textArea = textArea.withTrimmedLeft(4).withTrimmedRight(4);

            g.drawFittedText(label.getText(), textArea, label.getJustificationType(),
                             juce::jmax(1, (int)((float)textArea.getHeight() / font.getHeight())),
                             label.getMinimumHorizontalScale());
        }

        if (isSliderTextBox)
        {
            g.setColour(outline);
            g.drawRoundedRectangle(bounds, 4.0f, isLightMode ? 1.15f : 1.0f);
        }
        return;
    }

    g.setColour(label.findColour(juce::Label::backgroundColourId));
    g.fillRoundedRectangle(bounds, 4.f);

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

    g.drawRoundedRectangle(bounds, 4.f, isLightMode ? 1.2f : 1.f);
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
        g.fillRoundedRectangle(0, 0, width, height, 4.f);
    }
}

void MyLookAndFeel::drawTextEditorOutline(juce::Graphics &g, int width, int height,
                                          juce::TextEditor &textEditor)
{
    g.setColour(isLightMode ? juce::Colour(0xff8798ad) : juce::Colour(0xff282c35));
    g.drawRoundedRectangle(0, 0, width, height, 4.f, isLightMode ? 1.2f : 1.f);
}
