#include "PluginEditor.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), 
    bodyLAF (juce::Colour (0xffD8953B)),
    warmthLAF (juce::Colour (0xffC56B47)), 
    presenceLAF (juce::Colour (0xff29C1BE)),
    airLAF (juce::Colour (0xff71B8FF)),

    bodyKnob ("Body", "Thin", "Thick"),
    warmthKnob ("Warmth", "Crisp", "Rich"),
    presenceKnob ("Presence", "Distant", "Forward"),
    airKnob ("Air", "Closed", "Open")
{
    juce::ignoreUnused (processorRef);

    
    bodyValue.reset (60.0, 0.15);
    warmthValue.reset (60.0, 0.15);
    presenceValue.reset (60.0, 0.15);
    airValue.reset (60.0, 0.15);

    bodyAttachment =std::make_unique<SliderAttachment> (
            processorRef.apvts,
            ParamIDs::body,
            bodyKnob.getSlider());

    warmthAttachment = std::make_unique<SliderAttachment> (
        processorRef.apvts,
        ParamIDs::warmth,
        warmthKnob.getSlider());

    presenceAttachment = std::make_unique<SliderAttachment> (
        processorRef.apvts,
        ParamIDs::presence,
        presenceKnob.getSlider());

    airAttachment = std::make_unique<SliderAttachment> (
        processorRef.apvts,
        ParamIDs::air,
        airKnob.getSlider());

    bodyKnob.getSlider()
        .setLookAndFeel (&bodyLAF);
    warmthKnob.getSlider()
        .setLookAndFeel (&warmthLAF);
    presenceKnob.getSlider()
        .setLookAndFeel (&presenceLAF);
    airKnob.getSlider()
        .setLookAndFeel (&airLAF);

    setSize (900, 900);
    addAndMakeVisible (tonalCompass);
    addAndMakeVisible (bodyKnob);
    addAndMakeVisible (warmthKnob);
    addAndMakeVisible (presenceKnob);
    addAndMakeVisible (airKnob);

    startTimerHz (60);

    #if JUCE_DEBUG
    addAndMakeVisible (inspectButton);

    // this chunk of code instantiates and opens the melatonin inspector
    inspectButton.onClick = [&] {
        if (!inspector)
        {
            inspector = std::make_unique<melatonin::Inspector> (*this);
            inspector->onClose = [this]() { inspector.reset(); };
        }

        inspector->setVisible (true);
    };
#endif
}


PluginEditor::~PluginEditor()
{
    bodyKnob.getSlider()
        .setLookAndFeel (nullptr);

    warmthKnob.getSlider()
        .setLookAndFeel (nullptr);

    presenceKnob.getSlider()
        .setLookAndFeel (nullptr);

    airKnob.getSlider()
        .setLookAndFeel (nullptr);
}

void PluginEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour (0xff121417));
}

   

void PluginEditor::resized()
{
    // layout the positions of your child components here
    auto bounds = getLocalBounds().reduced (20);

    

    auto compassSize =
        juce::Rectangle<int> (
            bounds.getCentreX() - 250,
            bounds.getCentreY() - 250,
            500,
            500);
    tonalCompass.setBounds (compassSize);

    constexpr int knobSize = 110;
    constexpr int gap = 20;

   auto center = compassSize.getCentre();

    //Perceptual Knobs
    airKnob.setBounds(
        center.x - knobSize / 2,
        compassSize.getY() - gap - knobSize - 20,
        knobSize,
        knobSize);
    bodyKnob.setBounds (
        center.x - knobSize / 2,
        compassSize.getBottom() + gap + 20,
        knobSize,
        knobSize);
    warmthKnob.setBounds (
        compassSize.getX() - gap - knobSize,
        center.y - knobSize / 2,
        knobSize,
        knobSize);
    presenceKnob.setBounds (
        compassSize.getRight() + gap,
        center.y - knobSize / 2,
        knobSize,
        knobSize);


    
    #if JUCE_DEBUG
        inspectButton.setBounds (0,0,100,50);
    #endif
}

void PluginEditor::timerCallback()
{

    auto normalize = [] (float value) {
        return juce::jmap (
            value,
            -100.0f,
            100.0f,
            0.0f,
            1.0f);
    };

    bodyValue.setTargetValue (
        normalize (
            processorRef.apvts
                .getRawParameterValue (
                    ParamIDs::body)
                ->load()));

      warmthValue.setTargetValue (
        normalize (
            processorRef.apvts
                .getRawParameterValue (
                    ParamIDs::warmth)
                ->load()));

      presenceValue.setTargetValue (
        normalize (
            processorRef.apvts
                .getRawParameterValue (
                    ParamIDs::presence)
                ->load()));

      airValue.setTargetValue (
        normalize (
            processorRef.apvts
                .getRawParameterValue (
                    ParamIDs::air)
                ->load()));

      tonalCompass.setValues (
          bodyValue.getNextValue(),
          warmthValue.getNextValue(),
          presenceValue.getNextValue(),
          airValue.getNextValue());
}