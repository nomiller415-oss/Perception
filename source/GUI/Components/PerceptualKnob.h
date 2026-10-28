#pragma once

#include <juce_gui_basics/juce_gui_basics.h>


class PerceptualKnob : public juce::Component
{
public:
	PerceptualKnob(const juce::String& title,
				   const juce::String& minLabel,
				   const juce::String& maxLabel);
	~PerceptualKnob() override;

	juce::Slider& getSlider();

    juce::Colour getAccentColour() const;

	void paint (juce::Graphics&) override;
    void resized() override;

private:

	juce::String parameterName;
    juce::String minText;
    juce::String maxText;

    juce::Slider slider;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PerceptualKnob)
};
