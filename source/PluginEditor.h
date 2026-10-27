#pragma once

#include "PluginProcessor.h"
#if JUCE_DEBUG
    #include "melatonin_inspector/melatonin_inspector.h"
#endif


//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor
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

    juce::WebBrowserComponent webView;

    // Melatonin Inspector for debugging and UI Design
    #if JUCE_DEBUG
        std::unique_ptr<melatonin::Inspector> inspector;
        juce::TextButton inspectButton { "Inspect the UI" };
    #endif
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
