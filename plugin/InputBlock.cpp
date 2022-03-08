#include "InputBlock.h"

InputBlock::InputBlock(const juce::String &name, const juce::AudioProcessorValueTreeState &state)
    : Block(name)
{
    using type = ReverbSlider::Type;
    addParameter("blend", state.getParameter("InputMix"), type::Circle);
    addParameter("predelay", state.getParameter("PreDelay"), type::Circle);
    addParameter("low cut", state.getParameter("HighPass"), type::Circle);
    addParameter("high cut", state.getParameter("LowPass"), type::Circle);
    // button.setToggleState();
    // auto param = state.getParameter("DiffusionEnabled");
    // button_attachment =
    // std::make_unique<juce::ButtonParameterAttachment>(*dynamic_cast<juce::RangedAudioParameter*>(param),
    // button); addAndMakeVisible(button);
    auto param = state.getParameter("LowPassEnabled");
    LowButton_attachment = std::make_unique<juce::ButtonParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(param), lowButton);
    // lowButton.setLookAndFeel(&look);
    addAndMakeVisible(lowButton);
    auto param_ = state.getParameter("HiPassEnabled");
    HighButton_attachment = std::make_unique<juce::ButtonParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(param_), HighButton);
    // lowButton.setLookAndFeel(&look);
    addAndMakeVisible(HighButton);
}

void InputBlock::resized()
{
    // button.setBounds(2, 2, 30, 30);
    layout();
    auto x1 = sliders[2]->getX() + 3 * sliders[2]->getWidth() / 4.0;
    auto x2 = sliders[3]->getX() + 3 * sliders[3]->getWidth() / 4.0;
    auto y1 = sliders[2]->getY() * 0.8;
    auto y2 = sliders[3]->getY() * 0.8;
    auto size = sliders[2]->getWidth();
    auto buttonSize = std::min(30.0,size * 0.40);
    lowButton.setSize(buttonSize, buttonSize);
    lowButton.setCentrePosition(sliders[3]->getRight(), y2);
    // lowButton.setCentrePosition(x2, y2);
    HighButton.setSize(buttonSize, buttonSize);
    HighButton.setCentrePosition(sliders[2]->getRight(), y1);
    return;
//     auto main = getLocalBounds();
//     main.removeFromTop(50);
//     /*
//     auto flex = juce::FlexBox();
//     using Fl = juce::FlexBox;
//     //flex.flexWrap = Fl::Wrap::wrap;
//     flex.justifyContent = Fl::JustifyContent::spaceAround;
//     //flex.alignItems = Fl::AlignItems::flexStart;
//     flex.alignContent = Fl::AlignContent::center;
//     //flex.alignContent = Fl::AlignContent::stretch;
//     auto items = juce::Array<juce::FlexItem>();
//
//     for (auto& i : sliders) {
//         auto slider = i.get();
//         items.add(juce::FlexItem(size, size, *slider));
//     }
//     flex.items = items;
//     flex.performLayout(main);
// */
//     using Track = juce::Grid::TrackInfo;
//     using Fr = juce::Grid::Fr;
//     using Px = juce::Grid::Px;
//     juce::Grid grid;
//     for (auto &i : sliders)
//     {
//         auto slider = i.get();
//         grid.items.add(juce::GridItem(*slider).withSize(size, size));
//     }
//     /*
//     grid.items = {juce::GridItem(sliders["InputMix"]), juce::GridItem(sliders["HighPass"]),
//                        juce::GridItem(labels["InputMix"]), juce::GridItem(labels["HighPass"]),
//                        juce::GridItem(sliders["PreDelay"]), juce::GridItem(sliders["LowPass"]),
//                        juce::GridItem(labels["PreDelay"]), juce::GridItem(labels["LowPass"])};
//                        */
//     grid.templateColumns = {Track(Fr(1)), Track(Fr(1))};
//     grid.templateRows = {Track(Fr(1)), Track(Fr(1))};
//
//     // grid.alignContent = juce::Grid::AlignContent::start;
//     grid.alignItems = juce::Grid::AlignItems::start;
//     grid.justifyItems = juce::Grid::JustifyItems::center;
//     // grid.columnGap = Px(24);
//     // grid.rowGap = Px(24);
//     grid.performLayout(main);
//     for (size_t i = 0; i < sliders.size(); i++)
//     {
//         auto &slider = sliders[i];
//         auto &label = labels[i];
//         label->setCentrePosition(slider->getBounds().getCentreX(),
//                                  slider->getBounds().getBottom() + 8);
//     }
}
