#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class HeaderBar : public juce::Component
{
public:
	HeaderBar();
	~HeaderBar() override;

	void paint (juce::Graphics& g) override;
	void resized() override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HeaderBar)
};
