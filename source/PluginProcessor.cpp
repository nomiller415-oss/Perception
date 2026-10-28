#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
PluginProcessor::PluginProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
                      apvts (*this,
                          nullptr,
                          "PARAMETERS",
                          createParameterLayout())
{
}

PluginProcessor::~PluginProcessor()
{
}

//==============================================================================
const juce::String PluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PluginProcessor::getCurrentProgram()
{
    return 0;
}

void PluginProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String PluginProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    // Steinberg's VST3 validator fails plugins whose single default program has no name
    return "Default";
}

void PluginProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32> (samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32> (getTotalNumOutputChannels());

    eqEngine.prepare (spec);
}

void PluginProcessor::releaseResources()
{
    eqEngine.reset();
}

bool PluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void PluginProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    eqEngine.updateParameters (*apvts.getRawParameterValue (ParamIDs::body), 
        *apvts.getRawParameterValue (ParamIDs::warmth));
    eqEngine.process (buffer);

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        juce::ignoreUnused (channelData);
        // ..do something to the data...
    }
}

//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor (*this);
}

//==============================================================================
void PluginProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    
    auto state = apvts.copyState();

    std::unique_ptr<juce::XmlElement> xml (
        state.createXml());

    copyXmlToBinary (*xml, destData);
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml (
        getXmlFromBinary (data,
            sizeInBytes));

    if (xml.get() != nullptr)
    {
        if (xml->hasTagName (
                apvts.state.getType()))
        {
            apvts.replaceState (
                juce::ValueTree::fromXml (
                    *xml));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout
PluginProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add (std::make_unique<juce::AudioParameterFloat>(
        ParamIDs::body, "Body", juce::NormalisableRange<float> (
            -100.0f, 100.0f, 1.0f), 0.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat>(
        ParamIDs::warmth, "Warmth", juce::NormalisableRange<float> (
            -100.0f, 100.0f, 1.0f), 0.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat>(
        ParamIDs::presence, "Presence", juce::NormalisableRange<float> (
            -100.0f, 100.0f, 1.0f), 0.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat>(
        ParamIDs::air, "Air", juce::NormalisableRange<float> (
            -100.0f, 100.0f, 1.0f), 0.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        ParamIDs::output, "Output", juce::NormalisableRange<float> (
            -24.0f, 24.0f, 0.1f), 0.0f));

    layout.add (std::make_unique<juce::AudioParameterBool> (
        ParamIDs::bypass,"Bypass",false));


    return layout;
}