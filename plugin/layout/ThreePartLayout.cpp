#include "ThreePartLayout.h"
void ThreePartLayout::placeUIs(juce::Slider *numbox_1, juce::Slider *numbox_2,
                               juce::Slider *slider_1, juce::Slider *slider_2,
                               juce::Slider *mod_amt, juce::Slider *mod_rate, juce::Button *button,
                               const juce::Rectangle<int> &bound)
{
    const float ratio = bound.getWidth() / WIDTH > bound.getHeight() / HEIGHT
                            ? bound.getHeight() / HEIGHT
                            : bound.getWidth() / WIDTH;
    const int left = 97;
    const int middle = 170;
    const int right = 117;
    const float gutter =
        (bound.getWidth() - ratio * 2 * X_PADDING - (left + middle + right) * ratio) * 0.5;
    const float y1 = 94 * ratio + (bound.getHeight() - ratio * HEIGHT);
    const float y2 = 165 * ratio + (bound.getHeight() - ratio * HEIGHT);

    if (numbox_1 != nullptr)
    {
        numbox_1->setBounds(X_PADDING * ratio, y1, 97 * ratio, 40 * ratio);
    }
    numbox_2->setBounds(X_PADDING * ratio, y2, 97 * ratio, 40 * ratio);
    slider_1->setBounds(numbox_2->getRight() + gutter, y1, 65 * ratio, 65 * ratio);
    slider_2->setBounds(slider_1->getRight() + 41 * ratio, y1, 65 * ratio, 65 * ratio);
    mod_amt->setBounds(bound.getWidth() - X_PADDING * ratio - 40 * ratio, y1, 40 * ratio,
                       40 * ratio);
    mod_rate->setBounds(bound.getWidth() - X_PADDING * ratio - 40 * ratio, y2, 40 * ratio,
                        40 * ratio);

    if (button != nullptr)
    {
        button->setBounds(slider_2->getRight() - 25 * ratio, Y_PADDING * ratio, 25 * ratio,
                          25 * ratio);
    }
}
