#include "Layout.h"
#include "../MyLookAndFeel.h"

void Layout::drawTextUnderSlider(juce::Graphics &g, ReverbSlider *slider,
                                 const juce::Rectangle<int> &bound, int gap)
{
    const float ratio = bound.getWidth() / WIDTH > bound.getHeight() / HEIGHT
                            ? bound.getHeight() / HEIGHT
                            : bound.getWidth() / WIDTH;
    g.setColour(juce::Colour(0xffd6d6d6));

    auto font = MyLookAndFeel::getCustomFont().withHeight(38 * ratio);
    g.setFont(font);
    auto rec = juce::Rectangle<int>(slider->getX(), slider->getBottom() + gap * ratio,
                                    slider->getWidth(), 30 * ratio)
                   .withSizeKeepingCentre(slider->getWidth() * 4, 30 * ratio);

    g.drawFittedText(slider->getName(), rec, juce::Justification::centred, 1);
}
void Layout::drawTextRightToSlider(juce::Graphics &g, juce::Component *slider,
                                   const juce::Rectangle<int> &bound)
{
    const float ratio = bound.getWidth() / WIDTH > bound.getHeight() / HEIGHT
                            ? bound.getHeight() / HEIGHT
                            : bound.getWidth() / WIDTH;

    g.setColour(juce::Colour(0xffd6d6d6));

    auto font = MyLookAndFeel::getCustomFont().withHeight(38 * ratio);
    g.setFont(font);
    auto name = slider->getName();
    auto width = g.getCurrentFont().getStringWidth(name);
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
    g.setColour(juce::Colour(0xffd6d6d6));

    auto font = MyLookAndFeel::getCustomFont().withHeight(38 / 22.f * 38 * ratio);
    g.setFont(font);

    g.drawFittedText(name, X_PADDING * ratio, Y_PADDING * ratio, 200 * ratio, 47 * ratio,
                     juce::Justification::centredLeft, 1);
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
                           juce::Slider *slider_4, const juce::Rectangle<int> &bound)
{
    const float ratio = bound.getWidth() / WIDTH > bound.getHeight() / HEIGHT
                            ? bound.getHeight() / HEIGHT
                            : bound.getWidth() / WIDTH;
    const auto x_sliderSize = 25 * ratio;
    const auto y_sliderSize = 87 * ratio;
    const float gutter = (bound.getWidth() - ratio * 2 * X_PADDING - x_sliderSize * 4) / 3.f;
    const float y = 77 * ratio + (bound.getHeight() - ratio * HEIGHT);

    slider_1->setBounds(X_PADDING * ratio, y, x_sliderSize, y_sliderSize);
    slider_2->setBounds(slider_1->getRight() + gutter, y, x_sliderSize, y_sliderSize);
    slider_3->setBounds(slider_2->getRight() + gutter, y, x_sliderSize, y_sliderSize);
    slider_4->setBounds(slider_3->getRight() + gutter, y, x_sliderSize, y_sliderSize);
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
    const auto thinkness = 1.f;
    g.setColour(juce::Colour(0xffd6d6d6));
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

    diagram->setTransform(juce::AffineTransform::scale(ratio, ratio, x, y));
    diagram->setBounds(x, y, width, height);
    slider->setBounds(20 * ratio, 158 * ratio, 148 * ratio, 40 * ratio);
    button->setBounds(470 * ratio, Y_PADDING * ratio, 50 * ratio, 25 * ratio);
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
void HeaderLayout::drawTitle(juce::Graphics &g, const ::juce::String &name,
                             const juce::Rectangle<int> &bound)
{
    const float ratio = bound.getWidth() / WIDTH > bound.getHeight() / HEIGHT
                            ? bound.getHeight() / HEIGHT
                            : bound.getWidth() / WIDTH;
    g.setColour(juce::Colour(0xffd6d6d6));

    auto font = MyLookAndFeel::getCustomFont().withHeight(38 / 22.f * 62 * ratio);
    g.setFont(font);

    g.drawFittedText(name, X_PADDING * ratio, 0, bound.getWidth(), bound.getHeight(),
                     juce::Justification::centredLeft, 1);
}
void HeaderLayout::placeUIs(juce::Slider *cross_seed, juce::Button *interpolation,
                            juce::ComboBox *comboBox, const juce::Rectangle<int> &bound)
{
    const float ratio = bound.getWidth() / WIDTH > bound.getHeight() / HEIGHT
                            ? bound.getHeight() / HEIGHT
                            : bound.getWidth() / WIDTH;
    const auto sliderSize = 40 * ratio;
    const auto buttonSize = 19 * ratio;
    const float x1 = 655 * ratio;
    const float y1 = 35 * ratio;
    const float x2 = 943 * ratio;
    const float y2 = 46 * ratio;
    const float x3 = 1100 * ratio;
    const float y3 = 40 * ratio;
    cross_seed->setBounds(x1, y1, sliderSize, sliderSize);
    interpolation->setBounds(x2, y2, buttonSize, buttonSize);
    comboBox->setBounds(x3, y3, 200 * ratio, 35 * ratio);
}
