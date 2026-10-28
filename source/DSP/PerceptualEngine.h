#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

class PerceptualEQEngine
{
public:
    void prepare (const juce::dsp::ProcessSpec& spec);

    void process (juce::AudioBuffer<float>& buffer);
    void reset()
    {
        bodyShelf.reset();
        bodyBell.reset();

        warmthBell1.reset();
        warmthBell2.reset();


    }
    void updateParameters(float body, float warmth);


private:
    double sampleRate = 48000.0;

    float bodyValue = 0.0f;
    float warmthValue = 0.0f;

    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
        juce::dsp::IIR::Coefficients<float>>
        bodyShelf;

    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
        juce::dsp::IIR::Coefficients<float>>
        bodyBell;

    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
        juce::dsp::IIR::Coefficients<float>>
        warmthBell1;

    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
        juce::dsp::IIR::Coefficients<float>>
        warmthBell2;

};