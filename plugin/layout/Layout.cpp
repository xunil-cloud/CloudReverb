#ifdef _MSC_VER
#pragma warning(disable: 4702 4244 4305 4458 4100)
#endif

#include "Layout.h"
#include "../MyLookAndFeel.h"

void Layout::drawTextUnderSlider(juce::Graphics &g, ReverbSlider *slider,
                                 const juce::Rectangle<int> &bound, int gap)
{
    const float ratio = bound.getWidth() / WIDTH > bound.getHeight() / HEIGHT
                            ? bound.getHeight() / HEIGHT
                            : bound.getWidth() / WIDTH;
    bool isLight = MyLookAndFeel::isLightMode;
    g.setColour(isLight ? juce::Colour(0xff1a2634) : juce::Colour(0xffd6d6d6));

    auto font = MyLookAndFeel::getCustomFont().withHeight(24 * ratio);
    g.setFont(font);
    auto rec = juce::Rectangle<int>(slider->getX(), slider->getBottom() + gap * ratio,
                                    slider->getWidth(), 30 * ratio)
                   .withSizeKeepingCentre(120 * ratio, 30 * ratio);

    g.drawFittedText(slider->getName(), rec, juce::Justification::centred, 1);
}
void Layout::drawTextRightToSlider(juce::Graphics &g, juce::Component *slider,
                                   const juce::Rectangle<int> &bound)
{
    const float ratio = bound.getWidth() / WIDTH > bound.getHeight() / HEIGHT
                            ? bound.getHeight() / HEIGHT
                            : bound.getWidth() / WIDTH;
    bool isLight = MyLookAndFeel::isLightMode;
    g.setColour(isLight ? juce::Colour(0xff1a2634) : juce::Colour(0xffd6d6d6));

    auto font = MyLookAndFeel::getCustomFont().withHeight(24 * ratio);
    g.setFont(font);
    auto name = slider->getName();
    auto width = juce::GlyphArrangement::getStringWidth(g.getCurrentFont(), name);
    auto gap = 12;

    g.drawFittedText(name, slider->getX() - (width + gap * ratio), slider->getY(),
                     (width + gap * ratio), slider->getHeight(), juce::Justification::centredLeft,
                     1);
}

void Layout::drawTitle(juce::Graphics &g, const ::juce::String &name,
                       const juce::Rectangle<int> &bound)
{
    const float ratio = bound.getWidth() / WIDTH > bound.getHeight() / HEIGHT
                            ? bound.getHeight() / HEIGHT
                            : bound.getWidth() / WIDTH;
    bool isLight = MyLookAndFeel::isLightMode;
    g.setColour(isLight ? juce::Colour(0xff0d1b2a) : juce::Colour(0xffffffff));

    if (name == "CloudReverb")
    {
        auto font = MyLookAndFeel::getTitleFont().withHeight(38 / 22.f * 32 * ratio);
        g.setFont(font);
        g.drawFittedText(name, X_PADDING * ratio, Y_PADDING * ratio, 400 * ratio, 47 * ratio,
                         juce::Justification::centredLeft, 1);
    }
    else
    {
        auto font = MyLookAndFeel::getCustomFont().withHeight(32 * ratio);
        g.setFont(font);
        g.drawFittedText(name, X_PADDING * ratio, Y_PADDING * ratio, 350 * ratio, 47 * ratio,
                         juce::Justification::centredLeft, 1);
    }
}

void Layout::drawNumberBoxTitle(juce::Graphics &g, juce::Slider *slider, const juce::String &title,
                                const juce::Rectangle<int> &bound)
{
    const float ratio = bound.getWidth() / WIDTH > bound.getHeight() / HEIGHT
                            ? bound.getHeight() / HEIGHT
                            : bound.getWidth() / WIDTH;
    bool isLight = MyLookAndFeel::isLightMode;
    g.setColour(isLight ? juce::Colour(0xff3d4f63) : juce::Colour(0xff8a9ba8));
    auto font = MyLookAndFeel::getCustomFont().withHeight(20 * ratio);
    g.setFont(font);

    auto rec = juce::Rectangle<int>(slider->getX(), slider->getY() - 26 * ratio, slider->getWidth(), 24 * ratio);
    g.drawFittedText(title, rec, juce::Justification::centredLeft, 1);
}

void Layout::drawButtonTitle(juce::Graphics &g, juce::Button *button, const juce::String &title,
                             const juce::Rectangle<int> &bound)
{
    const float ratio = bound.getWidth() / WIDTH > bound.getHeight() / HEIGHT
                            ? bound.getHeight() / HEIGHT
                            : bound.getWidth() / WIDTH;
    bool isLight = MyLookAndFeel::isLightMode;
    g.setColour(isLight ? juce::Colour(0xff3d4f63) : juce::Colour(0xff8a9ba8));
    auto font = MyLookAndFeel::getCustomFont().withHeight(22 * ratio);
    g.setFont(font);

    auto rec = juce::Rectangle<int>(button->getX() - (int)(100 * ratio), button->getY(), (int)(92 * ratio), button->getHeight());
    g.drawFittedText(title, rec, juce::Justification::centredRight, 1);
}

void TapLayout::placeUIs(juce::Slider *numbox_1, juce::Slider *numbox_2, juce::Slider *slider_1,
                         juce::Slider *slider_2, juce::Slider *slider_3,
                         const juce::Rectangle<int> &bound)
{
    const float ratio = bound.getWidth() / WIDTH > bound.getHeight() / HEIGHT
                            ? bound.getHeight() / HEIGHT
                            : bound.getWidth() / WIDTH;
    const float gutter =
        (bound.getWidth() - ratio * 2 * X_PADDING - (97 + 65 * 2 + 40) * ratio) / 3.f;
    const float y1 = 94 * ratio + (bound.getHeight() - ratio * HEIGHT);
    const float y2 = 165 * ratio + (bound.getHeight() - ratio * HEIGHT);

    numbox_1->setBounds(X_PADDING * ratio, y1, 97 * ratio, 40 * ratio);
    numbox_2->setBounds(X_PADDING * ratio, y2, 97 * ratio, 40 * ratio);

    slider_1->setBounds(numbox_2->getRight() + gutter, y1, 65 * ratio, 65 * ratio);
    slider_2->setBounds(slider_1->getRight() + gutter, y1, 65 * ratio, 65 * ratio);
    slider_3->setBounds(slider_2->getRight() + gutter, slider_2->getBottom() - 40 * ratio,
                        40 * ratio, 40 * ratio);
}

void InputLayout::placeUIs(juce::Slider *slider_1, juce::Slider *slider_2, juce::Slider *slider_3,
                           juce::Slider *slider_4, juce::Button *button_3, juce::Button *button_4,
                           const juce::Rectangle<int> &bound)
{
    const float ratio = bound.getWidth() / WIDTH > bound.getHeight() / HEIGHT
                            ? bound.getHeight() / HEIGHT
                            : bound.getWidth() / WIDTH;
    const auto sliderSize = 52 * ratio;
    const float gutter = (bound.getWidth() - ratio * 2 * X_PADDING - sliderSize * 4) / 3.f;
    const float y = 108 * ratio + (bound.getHeight() - ratio * HEIGHT);

    slider_1->setBounds(X_PADDING * ratio, y, sliderSize, sliderSize);
    slider_2->setBounds(slider_1->getRight() + gutter, y, sliderSize, sliderSize);
    slider_3->setBounds(slider_2->getRight() + gutter, y, sliderSize, sliderSize);
    slider_4->setBounds(slider_3->getRight() + gutter, y, sliderSize, sliderSize);

    const auto buttonSize = 19 * ratio;
    const auto x_offset = -11 * ratio;
    const auto y_offset = -5 * ratio - buttonSize;
    button_3->setBounds(slider_3->getX() + x_offset, slider_3->getY() + y_offset, buttonSize,
                        buttonSize);
    button_4->setBounds(slider_4->getX() + x_offset, slider_4->getY() + y_offset, buttonSize,
                        buttonSize);
}
void MixerLayout::placeUIs(juce::Slider *slider_1, juce::Slider *slider_2, juce::Slider *slider_3,
                           juce::Slider *slider_4, juce::Slider *slider_5, const juce::Rectangle<int> &bound)
{
    const float ratio = bound.getWidth() / WIDTH > bound.getHeight() / HEIGHT
                            ? bound.getHeight() / HEIGHT
                            : bound.getWidth() / WIDTH;
    const auto x_sliderSize = 25 * ratio;
    const auto y_sliderSize = 87 * ratio;
    const float gutter = (bound.getWidth() - ratio * 2 * X_PADDING - x_sliderSize * 5) / 4.f;
    const float y = 77 * ratio + (bound.getHeight() - ratio * HEIGHT);

    slider_1->setBounds(X_PADDING * ratio, y, x_sliderSize, y_sliderSize);
    slider_2->setBounds(slider_1->getRight() + gutter, y, x_sliderSize, y_sliderSize);
    slider_3->setBounds(slider_2->getRight() + gutter, y, x_sliderSize, y_sliderSize);
    slider_4->setBounds(slider_3->getRight() + gutter, y, x_sliderSize, y_sliderSize);
    slider_5->setBounds(slider_4->getRight() + gutter, y, x_sliderSize, y_sliderSize);
}
void EqLayout::placeUIs(juce::Slider *low_1, juce::Slider *low_2, juce::Slider *high_1,
                        juce::Slider *high_2, juce::Slider *cutoff, juce::Button *low_button,
                        juce::Button *high_button, juce::Button *cutoff_button,
                        const juce::Rectangle<int> &bound)
{
    const float ratio = bound.getWidth() / WIDTH > bound.getHeight() / HEIGHT
                            ? bound.getHeight() / HEIGHT
                            : bound.getWidth() / WIDTH;
    const auto sliderSize = 52 * ratio;
    const float y1 = 101 * ratio + (bound.getHeight() - ratio * HEIGHT);
    const float y2 = 226 * ratio + (bound.getHeight() - ratio * HEIGHT);
    const float y3 = 352 * ratio + (bound.getHeight() - ratio * HEIGHT);
    const float x1 = 70 * ratio;
    const float x2 = 139 * ratio;
    const float x3 = 207 * ratio;

    low_1->setBounds(x1, y1, sliderSize, sliderSize);
    low_2->setBounds(x1, y2, sliderSize, sliderSize);
    high_1->setBounds(x3, y1, sliderSize, sliderSize);
    high_2->setBounds(x3, y2, sliderSize, sliderSize);
    cutoff->setBounds(x2, y3, sliderSize, sliderSize);

    const auto buttonSize = 19 * ratio;
    const auto x_offset = -11 * ratio;
    const auto y_offset = -5 * ratio - buttonSize;
    cutoff_button->setBounds(cutoff->getX() + x_offset, cutoff->getY() + y_offset, buttonSize,
                             buttonSize);
    const auto low_high_button_y = 180 * ratio + (bound.getHeight() - ratio * HEIGHT);
    low_button->setBounds(X_PADDING * ratio, low_high_button_y, buttonSize, buttonSize);
    high_button->setBounds(bound.getWidth() - buttonSize - X_PADDING * ratio, low_high_button_y,
                           buttonSize, buttonSize);
}
void EqLayout::drawLines(juce::Graphics &g, const juce::Rectangle<int> &bound)
{
    const float ratio = bound.getWidth() / WIDTH > bound.getHeight() / HEIGHT
                            ? bound.getHeight() / HEIGHT
                            : bound.getWidth() / WIDTH;
    const auto x1 = 49 * ratio;
    const auto x2 = 269 * ratio;
    const auto y1 = 127 * ratio;
    const auto y2 = 252 * ratio;
    const auto width = 12 * ratio;
    const auto height = 125 * ratio;
    const auto thinkness = MyLookAndFeel::isLightMode ? 1.8f : 1.f;
    g.setColour(MyLookAndFeel::isLightMode ? juce::Colour(0xff7a8ea5) : juce::Colour(0xffd6d6d6));
    g.drawRect(x1, y1, width, thinkness);
    g.drawRect(x1, y2, width, thinkness);
    g.drawRect(x1, y1, thinkness, height);

    g.drawRect(x2, y1, width, thinkness);
    g.drawRect(x2, y2, width, thinkness);
    g.drawRect(x2 + width - 1, y1, thinkness, height);
}
void RouteLayout::placeUIs(juce::Drawable *diagram, juce::Slider *slider, juce::Button *button,
                           const juce::Rectangle<int> &bound)
{
    const float ratio = bound.getWidth() / WIDTH > bound.getHeight() / HEIGHT
                            ? bound.getHeight() / HEIGHT
                            : bound.getWidth() / WIDTH;
    const auto x = 80 * ratio;
    const auto y = 86 * ratio;
    const auto width = 440 * ratio;
    const auto height = 119 * ratio;

    if (diagram != nullptr)
    {
        diagram->setTransform(juce::AffineTransform::scale(ratio, ratio, x, y));
        diagram->setBounds(x, y, width, height);
    }
    slider->setBounds((int)(20 * ratio), (int)(113 * ratio), (int)(135 * ratio), (int)(35 * ratio)); // Reverted to perfect vertical center alignment with middleY (131*ratio)!
    button->setBounds(bound.getWidth() - X_PADDING * ratio - 50 * ratio, Y_PADDING * ratio, 50 * ratio, 25 * ratio);
}
void RouteLayout::drawTextOndiagram(juce::Graphics &g, const juce::String &fist_block,
                                    const juce::String &second_block,
                                    const juce::Rectangle<int> &bound)
{
    const float ratio = bound.getWidth() / WIDTH > bound.getHeight() / HEIGHT
                            ? bound.getHeight() / HEIGHT
                            : bound.getWidth() / WIDTH;
    const int width = 110 * ratio;
    const int height = 41 * ratio;

    auto font = MyLookAndFeel::getCustomFont().withHeight(38 * ratio);
    g.setFont(font);
    g.setColour(juce::Colour(0xff2b2b2b));
    g.drawFittedText(fist_block, 166 * ratio, 96 * ratio, width, height,
                     juce::Justification::centred, 1);
    g.drawFittedText(second_block, 355 * ratio, 96 * ratio, width, height,
                     juce::Justification::centred, 1);
    g.drawFittedText("EQ", 230 * ratio, 167 * ratio, width, height, juce::Justification::centred,
                     1);
}
void RouteLayout::drawCustomDiagram(juce::Graphics &g, bool isPre, const juce::Rectangle<int> &bound)
{
    const float ratio = bound.getWidth() / WIDTH > bound.getHeight() / HEIGHT
                            ? bound.getHeight() / HEIGHT
                            : bound.getWidth() / WIDTH;

    auto boxWidth = 95 * ratio;
    auto boxHeight = 35 * ratio;
    auto upperY = 82 * ratio + boxHeight * 0.5f; // Exact center of top boxes
    auto lowerY = 145 * ratio + boxHeight * 0.5f; // Exact center of EQ box
    auto middleY = (upperY + lowerY) * 0.5f;
    auto startX = 160 * ratio;
    auto splitX = 175 * ratio;
    auto sumX = 445 * ratio;
    auto endX = 490 * ratio;

    juce::Colour routeColour = MyLookAndFeel::isLightMode
                                    ? juce::Colour(0xff1e88e5)  // Material blue for light theme
                                    : juce::Colour(0xffc78d06); // Deep elegant yellow for dark theme
    juce::Colour routeTextColour = MyLookAndFeel::isLightMode
                                        ? juce::Colour(0xffffffff)
                                        : juce::Colour(0xff000000);

    // 1. Draw solid route-coloured connecting lines
    g.setColour(routeColour);
    g.drawHorizontalLine((int)upperY, (float)splitX, (float)sumX);
    g.drawHorizontalLine((int)lowerY, (float)splitX, (float)sumX);
    g.drawHorizontalLine((int)middleY, (float)startX, (float)splitX); // Input line to split dot
    if (!isPre) g.drawHorizontalLine((int)middleY, (float)splitX, (float)sumX); // Middle direct line only in POST-EQ MIX!
    g.drawHorizontalLine((int)middleY, (float)sumX, (float)endX); // Output line

    g.drawVerticalLine((int)splitX, (float)upperY, (float)lowerY);
    g.drawVerticalLine((int)sumX, (float)upperY, (float)lowerY);

    auto dotSize = 8.0f * ratio;
    g.fillEllipse(splitX - dotSize * 0.5f, middleY - dotSize * 0.5f, dotSize, dotSize);

    juce::Path arrow;
    auto arrowSize = 12.0f * ratio;
    arrow.addTriangle(endX, middleY - arrowSize * 0.6f, endX + arrowSize, middleY, endX, middleY + arrowSize * 0.6f);
    g.fillPath(arrow);

    // 2. Draw Summation Circle (+)
    auto circleSize = 22.0f * ratio;
    juce::Rectangle<float> circleRect(sumX - circleSize * 0.5f, middleY - circleSize * 0.5f, circleSize, circleSize);
    g.setColour(routeColour);
    g.fillEllipse(circleRect);
    
    auto font = MyLookAndFeel::getCustomFont().withHeight(20 * ratio);
    g.setFont(font.withHeight(22 * ratio));
    g.setColour(routeTextColour);
    g.drawFittedText("+", circleRect.toNearestInt(), juce::Justification::centred, 1);

    // 3. Draw solid route-coloured boxes (No transparency, no stroke!)
    juce::String fist_block = isPre ? "Late Diff." : "Late Delay";
    juce::String second_block = isPre ? "Late Delay" : "Late Diff.";

    auto drawYellowBox = [&](float bx, float by, const juce::String &text) {
        juce::Rectangle<float> rect(bx, by, boxWidth, boxHeight);
        g.setColour(routeColour);
        g.fillRect(rect); // Solid route-coloured fill, no stroke!

        g.setFont(font);
        g.setColour(routeTextColour);
        g.drawFittedText(text, rect.toNearestInt(), juce::Justification::centred, 1);
    };

    drawYellowBox(195 * ratio, 82 * ratio, fist_block);
    drawYellowBox(310 * ratio, 82 * ratio, second_block);
    drawYellowBox(250 * ratio, 145 * ratio, "EQ");

    // 4. Draw elegant routing mode text below EQ box
    g.setColour(MyLookAndFeel::isLightMode ? juce::Colour(0xff3d4f63) : juce::Colour(0xff8a9ba8));
    auto modeFont = MyLookAndFeel::getCustomFont().withHeight(16 * ratio);
    g.setFont(modeFont);
    juce::String mixText = isPre ? "PRE-EQ MIX" : "POST-EQ MIX";
    g.drawFittedText(mixText, (int)(250 * ratio), (int)(145 * ratio + boxHeight + 2 * ratio), (int)boxWidth, (int)(18 * ratio), juce::Justification::centred, 1);
}
void HeaderLayout::drawTitle(juce::Graphics &g, const ::juce::String &name,
                             const juce::Rectangle<int> &bound)
{
    const float ratio = bound.getWidth() / WIDTH > bound.getHeight() / HEIGHT
                            ? bound.getHeight() / HEIGHT
                            : bound.getWidth() / WIDTH;
    bool isLight = MyLookAndFeel::isLightMode;

    g.setColour(isLight ? juce::Colour(0xff7f9fbd) : juce::Colour(0xffffffff));
    auto titleFont = MyLookAndFeel::getTitleFont().withHeight(52 * ratio);
    g.setFont(titleFont);
    g.drawFittedText(name, (int)(X_PADDING * ratio), (int)(12 * ratio), (int)(400 * ratio), (int)(55 * ratio), juce::Justification::centredLeft, 1);

    g.setColour(isLight ? juce::Colour(0xff3d4f63) : juce::Colour(0xff8a9ba8));
    auto verFont = MyLookAndFeel::getCustomFont().withHeight(20 * ratio);
    g.setFont(verFont);
    g.drawFittedText("v1.0", (int)(X_PADDING * ratio), (int)(66 * ratio), (int)(200 * ratio), (int)(25 * ratio), juce::Justification::centredLeft, 1);
}
void HeaderLayout::drawPresetTitle(juce::Graphics &g, juce::ComboBox *comboBox,
                                   const juce::Rectangle<int> &bound)
{
    const float ratio = bound.getWidth() / WIDTH > bound.getHeight() / HEIGHT
                            ? bound.getHeight() / HEIGHT
                            : bound.getWidth() / WIDTH;
    bool isLight = MyLookAndFeel::isLightMode;

    g.setColour(isLight ? juce::Colour(0xff3d4f63) : juce::Colour(0xff8a9ba8));
    auto font = MyLookAndFeel::getCustomFont().withHeight(22 * ratio);
    g.setFont(font);

    const int labelWidth = (int)(86 * ratio);
    const int gap = (int)(10 * ratio);
    auto labelBounds = juce::Rectangle<int>(comboBox->getX() - labelWidth - gap,
                                            comboBox->getY(),
                                            labelWidth,
                                            comboBox->getHeight());
    g.drawFittedText("Presets", labelBounds, juce::Justification::centredRight, 1);
}
void HeaderLayout::placeUIs(juce::Slider *cross_seed, juce::Button *interpolation,
                            juce::ComboBox *comboBox, const juce::Rectangle<int> &bound)
{
    const float ratio = bound.getWidth() / WIDTH > bound.getHeight() / HEIGHT
                            ? bound.getHeight() / HEIGHT
                            : bound.getWidth() / WIDTH;
    const auto sliderSize = 55 * ratio; // Prominent larger cross seed knob!
    const auto buttonSize = 19 * ratio;
    const float x1 = 645 * ratio;
    const float y1 = 28 * ratio;
    const float x2 = 910 * ratio;
    const float y2 = 46 * ratio;
    const float x3 = 1030 * ratio; // Right-aligned preset area with room for Undo/Redo and theme button
    const float y3 = 40 * ratio;
    cross_seed->setBounds(x1, y1, sliderSize, sliderSize);
    interpolation->setBounds(x2, y2, buttonSize, buttonSize);
    comboBox->setBounds(x3, y3, 220 * ratio, 35 * ratio);
}
