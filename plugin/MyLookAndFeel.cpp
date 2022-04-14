#include "MyLookAndFeel.h"

MyLookAndFeel::MyLookAndFeel()
    : juce::LookAndFeel_V4::LookAndFeel_V4({0xff2b2b2b, 0xff484848, 0xff2b2b2b, 0xff121212,
                                            0xffd6d6d6, 0xff225ebd, 0xfffff1ff, 0xff227dbd,
                                            0xffffffff})
{
    setColour(juce::Slider::trackColourId, juce::Colour(0xff225ebd));
    setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour(0xff383838));
    // setDefaultSansSerifTypefaceName("Roboto");
}
