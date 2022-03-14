#include "MyLookAndFeel.h"

MyLookAndFeel::MyLookAndFeel()
    : juce::LookAndFeel_V4::LookAndFeel_V4({0xff2b2b2b, 0xff484848, 0xff2b2b2b, 0xff121212,
                                            0xffd6d6d6, 0xff225ebd, 0xfffff1ff, 0xff227dbd,
                                            0xffffffff})
{
    // setColour(juce::Slider::thumbColourId, {255, 0, 153});
    // 0xff227dbd
    // setColour(juce::ResizableWindow::backgroundColourId, {234, 234, 234});
    // setColourScheme({0x303030, 0xff683838, 0x303030, 0xff46d6d6, 0xff16d6d6, 0xff225ebd,
    // 0xfffff1ff,
    //                  0xff181f22, 0xfffff1ff});
}
