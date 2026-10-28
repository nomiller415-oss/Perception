#pragma once

#include "PluginProcessor.h"
#include "GUI/Components/TonalCompass.h"
#include "GUI/Components/PerceptualKnob.h"
#include "GUI/Styles/LookAndFeel.h"
#if JUCE_DEBUG
    #include "melatonin_inspector/melatonin_inspector.h"
#endif


//==============================================================================
class PluginEditor
    : public juce::AudioProcessorEditor,
      public juce::Timer
{
public:
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor& processorRef;

    TonalCompass tonalCompass;

    void timerCallback() override;

    //LookandFeel
    PerceptionLookAndFeel bodyLAF;
    PerceptionLookAndFeel warmthLAF;
    PerceptionLookAndFeel presenceLAF;
    PerceptionLookAndFeel airLAF;

    //PerceptKnobs
    PerceptualKnob bodyKnob;
    PerceptualKnob warmthKnob;
    PerceptualKnob presenceKnob;
    PerceptualKnob airKnob;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<SliderAttachment> bodyAttachment;
    std::unique_ptr<SliderAttachment> warmthAttachment;
    std::unique_ptr<SliderAttachment> presenceAttachment;
    std::unique_ptr<SliderAttachment> airAttachment;

    juce::SmoothedValue<float> bodyValue;
    juce::SmoothedValue<float> warmthValue;
    juce::SmoothedValue<float> presenceValue;
    juce::SmoothedValue<float> airValue;

    // Melatonin Inspector for debugging and UI Design
    #if JUCE_DEBUG
        std::unique_ptr<melatonin::Inspector> inspector;
        juce::TextButton inspectButton { "Inspect the UI" };
    #endif
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
