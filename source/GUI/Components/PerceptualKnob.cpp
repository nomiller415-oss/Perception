#include "PerceptualKnob.h"
#include "../Styles/LookAndFeel.h"

PerceptualKnob::PerceptualKnob(const juce::String& title,
							   const juce::String& minLabel,
							   const juce::String& maxLabel)
	: parameterName(title), minText(minLabel), maxText(maxLabel)
{
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    slider.setRange (-100.0, 100.0, 1.0);

	addAndMakeVisible (slider);
}

PerceptualKnob::~PerceptualKnob()
{
}

void PerceptualKnob::resized()
{
    auto bounds = getLocalBounds();

    constexpr int titleHeight = 24;
    constexpr int footerHeight = 24;

    bounds.removeFromTop (titleHeight);
    bounds.removeFromBottom (footerHeight);

    const int size =
        juce::jmin (bounds.getWidth(),
            bounds.getHeight());

    auto sliderArea =
        juce::Rectangle<int> (
            size,
            size);

    sliderArea.setCentre (bounds.getCentre());

    slider.setBounds (sliderArea);
}

void PerceptualKnob::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds();

    g.setColour (getAccentColour().brighter(0.1f).withMultipliedSaturation(0.8f));

    g.setFont (
        juce::FontOptions (16.0f));

    g.drawFittedText (
        parameterName,
        bounds.removeFromTop (24),
        juce::Justification::centred,
        1);

    auto bottomArea =
        bounds.removeFromBottom (24);

    g.setFont (
        juce::FontOptions (12.0f));

    g.setColour (
        juce::Colour (0xff888d94));

    g.drawText (
        minText,
        bottomArea.removeFromLeft (
            bottomArea.getWidth() / 2),
        juce::Justification::left);

    g.drawText (
        maxText,
        bottomArea,
        juce::Justification::right);
}

juce::Slider& PerceptualKnob::getSlider()
{
	return slider;
}

juce::Colour PerceptualKnob::getAccentColour() const
{
    if (auto* laf =
            dynamic_cast<PerceptionLookAndFeel*> (
                &slider.getLookAndFeel()))
    {
        return laf->getAccentColor();
    }

    return juce::Colours::white;
}