#include "PerceptualEngine.h"

void PerceptualEQEngine::prepare (const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    bodyShelf.prepare (spec);
    bodyBell.prepare (spec);
    warmthBell1.prepare (spec);
    warmthBell2.prepare (spec);

    updateParameters(0.0f, 0.0f);

}
void PerceptualEQEngine::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    
    juce::dsp::ProcessContextReplacing<float> context (block);
    bodyShelf.process (context);
    bodyBell.process (context);

    warmthBell1.process (context);
    warmthBell2.process (context);

}

void PerceptualEQEngine::updateParameters (float body, float warmth)
{
    bodyValue = body;
    warmthValue = warmth;

    const float shelfGaindB = juce::jmap (body, -100.0f, 100.0f, -5.0f, 5.0f); // Map body to gain in dB
    const float bellGaindB = juce::jmap (body, -100.0f, 100.0f, -2.0f, 2.0f); // Map body to gain in dB

    const float warmthGaindB1 = juce::jmap (warmth, -100.0f, 100.0f, -2.5f, 2.5f); // Map warmth to gain in dB
    const float warmthGaindB2 = juce::jmap (warmth, -100.0f, 100.0f, -1.5f, 1.5f); // Map warmth to gain in dB

    *bodyShelf.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf (sampleRate, 90, 0.7071f, juce::Decibels::decibelsToGain (shelfGaindB));
    *bodyBell.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, 220, 0.55f, juce::Decibels::decibelsToGain (bellGaindB));

    *warmthBell1.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, 350, 0.8f, juce::Decibels::decibelsToGain (warmthGaindB1));
    *warmthBell2.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, 800, 0.7f, juce::Decibels::decibelsToGain (warmthGaindB2));


}