#include "EqBlock.h"
#include <cstddef>

EqBlock::EqBlock(const juce::String &name, const juce::AudioProcessorValueTreeState &state)
    : Block(name)
{
    using type = ReverbSlider::Type;
    addParameter("low freq.", state.getParameter("PostLowShelfFrequency"), type::Circle);
    addParameter("low gain", state.getParameter("PostLowShelfGain"), type::Circle);
    addParameter("high gain", state.getParameter("PostHighShelfGain"), type::Circle);
    addParameter("high freq.", state.getParameter("PostHighShelfFrequency"), type::Circle);
    addParameter("cutoff", state.getParameter("PostCutoffFrequency"), type::Circle);

    auto param = state.getParameter("LowShelfEnabled");
    LowButton_attachment = std::make_unique<juce::ButtonParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(param), lowButton);
    // lowButton.setLookAndFeel(&look);
    addAndMakeVisible(lowButton);
    auto param_ = state.getParameter("HighShelfEnabled");
    HighButton_attachment = std::make_unique<juce::ButtonParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(param_), HighButton);
    // lowButton.setLookAndFeel(&look);
    addAndMakeVisible(HighButton);
    param_ = state.getParameter("CutoffEnabled");
    cutoffButton_attachment = std::make_unique<juce::ButtonParameterAttachment>(
        *dynamic_cast<juce::RangedAudioParameter *>(param_), cutoffButton);
    // lowButton.setLookAndFeel(&look);
    header_ratio = 0.5;
    addAndMakeVisible(cutoffButton);
}

void EqBlock::resized()
{
    auto main = getLocalBounds();
    auto rec = main.withSizeKeepingCentre(getWidth() * 0.9, getHeight() * header_ratio *
                                                                (0.9 + 1 / header_ratio - 1));
    rec.removeFromTop(getHeaderSize());
    auto size = std::min(70.0, getHeight() * header_ratio * 0.3);
    /*
    auto flex = juce::FlexBox();
    using Fl = juce::FlexBox;
    //flex.flexWrap = Fl::Wrap::wrap;
    flex.justifyContent = Fl::JustifyContent::spaceAround;
    //flex.alignItems = Fl::AlignItems::flexStart;
    flex.alignContent = Fl::AlignContent::center;
    //flex.alignContent = Fl::AlignContent::stretch;
    auto items = juce::Array<juce::FlexItem>();

    for (auto& i : sliders) {
        auto slider = i.get();
        items.add(juce::FlexItem(size, size, *slider));
    }
    flex.items = items;
    flex.performLayout(main);
*/
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    using Px = juce::Grid::Px;
    juce::Grid grid;
    // for (auto &i : sliders)
    for (size_t i = 0; i < 4; i++)
    {
        auto slider = sliders[i].get();
        grid.items.add(juce::GridItem(*slider).withSize(size, size));
    }
    grid.items.add(juce::GridItem(*sliders[4].get()).withSize(size, size).withArea(3, 1, 3, 3));
    /*
    grid.items = {juce::GridItem(sliders["InputMix"]), juce::GridItem(sliders["HighPass"]),
                       juce::GridItem(labels["InputMix"]), juce::GridItem(labels["HighPass"]),
                       juce::GridItem(sliders["PreDelay"]), juce::GridItem(sliders["LowPass"]),
                       juce::GridItem(labels["PreDelay"]), juce::GridItem(labels["LowPass"])};
                       */
    grid.templateColumns = {Track(Fr(1)), Track(Fr(1))};
    grid.templateRows = {Track(Fr(1)), Track(Fr(1)), Track(Fr(1))};

    // grid.alignContent = juce::Grid::AlignContent::start;
    grid.alignItems = juce::Grid::AlignItems::center;
    grid.justifyItems = juce::Grid::JustifyItems::center;
    // grid.columnGap = Px(24);
    // grid.rowGap = Px(24);
    grid.performLayout(rec);
    for (size_t i = 0; i < sliders.size(); i++)
    {
        auto &slider = sliders[i];
        auto &label = labels[i];
        auto fontsize = std::min(rec.getWidth() * 0.05 * 1.5, size * 0.35);
        label->setFont(juce::Font("Roboto", fontsize, juce::Font::plain));
        label->setSize(float(rec.getWidth()) / 2.0, fontsize + 2);
        label->setCentrePosition(slider->getBounds().getCentreX(),
                                 slider->getBounds().getBottom() + fontsize * 0.75);
    }
    auto x1 = (sliders[0]->getX() + sliders[1]->getX()) / 2.0 + sliders[0]->getWidth() / 2.0;
    auto x2 = (sliders[2]->getX() + sliders[3]->getX()) / 2.0 + sliders[0]->getWidth() / 2.0;
    auto x3 = sliders[4]->getX() + sliders[4]->getWidth() + 30.0;
    // auto y = (sliders[0]->getY() + sliders[0]->getHeight() / 2.0);
    auto y1 = sliders[0]->getY() + 20;
    auto y2 = sliders[2]->getY() + 20;
    auto y3 = sliders[4]->getY() + 20;

    auto button_size = std::min(30.0, size * 0.40);
    lowButton.setSize(button_size, button_size);
    lowButton.setCentrePosition(x1, y1);
    HighButton.setSize(button_size, button_size);
    HighButton.setCentrePosition(x2, y2);
    cutoffButton.setSize(button_size, button_size);
    cutoffButton.setCentrePosition(x3, y3);
}
