#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "ColorPalette.h"

class PerceptionLookAndFeel : public juce::LookAndFeel_V4
{
public:
	PerceptionLookAndFeel(juce::Colour accentColor);
	~PerceptionLookAndFeel() override;

	void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
		float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
		juce::Slider& slider) override;

	juce::Colour getAccentColor() const;

private:
    juce::Colour accent;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PerceptionLookAndFeel)
};
