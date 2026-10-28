#pragma once

#include "juce_gui_basics/juce_gui_basics.h"

class TonalCompass : public juce::Component
{
public:
    TonalCompass();

    void setValues (float body,
        float warmth,
        float presence,
        float air);

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    float body = 0.5f;
    float warmth = 0.5f;
    float presence = 0.5f;
    float air = 0.5f;
};