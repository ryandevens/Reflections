/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ReflectionsAudioProcessor::ReflectionsAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts (*this, nullptr, "Parameters", createParameters())
#endif
{
    apvts.state.addListener (this);
}

ReflectionsAudioProcessor::~ReflectionsAudioProcessor()
{
}

//==============================================================================
const juce::String ReflectionsAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ReflectionsAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ReflectionsAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ReflectionsAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ReflectionsAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ReflectionsAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ReflectionsAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ReflectionsAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ReflectionsAudioProcessor::getProgramName (int index)
{
    return {};
}

void ReflectionsAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ReflectionsAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    reverbProcessor.init(sampleRate, samplesPerBlock);
    delayProcessor.init(sampleRate, samplesPerBlock);
    
    
    update();
}

void ReflectionsAudioProcessor::releaseResources()
{
    reverbProcessor.reset();
    delayProcessor.reset();
    //outputAnalyser.stopThread (1000);
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ReflectionsAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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
#endif

void ReflectionsAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    if (mustUpdateProcessing)
        update();
    
    
    // initially just buffers full of zeroes, then consider this the previous buffer
    AudioBuffer<float>& dBufferPrev = delayProcessor.getPreviousDelayBuffer();
    AudioBuffer<float>& vBufferPrev = reverbProcessor.getPreviousVerbBuffer();
    
    // process the audio buffer with the "previous" processorbuffer
    delayProcessor.process(buffer, vBufferPrev);
    reverbProcessor.process(buffer, dBufferPrev);
    
    AudioBuffer<float>& dBuffer = delayProcessor.getDelayBuffer();
    AudioBuffer<float>& vBuffer = reverbProcessor.getVerbBuffer();

    //buffer.clear();
    buffer.applyGain(0, buffer.getNumSamples(), 1.f-mix.get());
    for(int channel = 0; channel < totalNumOutputChannels; ++channel)
    {
        buffer.addFrom(channel, 0, dBuffer, channel, 0, buffer.getNumSamples(), delayOutputLevel.get() * mix.get());
        buffer.addFrom(channel, 0, vBuffer, channel, 0, buffer.getNumSamples(), verbOutputLevel.get() * mix.get());
    }
    
    // Here we store the previous buffer at the end of the process block to be ready for next block
    delayProcessor.storePreviousBuffer();
    reverbProcessor.storePreviousBuffer();
    
    if (syncButtonState)
    {
        playHead = this->getPlayHead();
        playHead->getCurrentPosition(currentPos);
        tempoSync.setTempo(currentPos.bpm, currentPos.timeSigNumerator, currentPos.timeSigDenominator);
    }
    
    //outputAnalyser.addAudioData (buffer, 0, getTotalNumOutputChannels());
}

//==============================================================================
bool ReflectionsAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ReflectionsAudioProcessor::createEditor()
{
    return new ReflectionsAudioProcessorEditor (*this);
}

//==============================================================================
void ReflectionsAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::ValueTree copyState = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml = copyState.createXml();
    copyXmlToBinary (*xml.get(), destData);
    
}

void ReflectionsAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml = getXmlFromBinary (data, sizeInBytes);
    juce::ValueTree copyState = juce::ValueTree::fromXml (*xml.get());
    apvts.replaceState (copyState);
}



//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReflectionsAudioProcessor();
}

void ReflectionsAudioProcessor::update()
{
    mustUpdateProcessing = false;
    
    auto vButtonState = apvts.getRawParameterValue("Verb Freeze");
    auto size = apvts.getRawParameterValue("Size");
    auto damp = apvts.getRawParameterValue("Damping");
    auto wide = apvts.getRawParameterValue("Width");
    auto freeze = 0.0f;
    
    auto verbInput = apvts.getRawParameterValue("Verb Input");
    auto verbOutput = apvts.getRawParameterValue("Verb Output");
    auto verbSend = apvts.getRawParameterValue("Verb Send");
    
    verbOutputLevel = *verbOutput;
    
    if (*vButtonState)
        freeze = 1.0f;
    else
        freeze = 0.0f;
    
    auto dButtonState = apvts.getRawParameterValue("Delay Link");
    auto dSyncState = apvts.getRawParameterValue("Sync");
    auto dTimeL = apvts.getRawParameterValue ("Time L");
    auto feedbackL = apvts.getRawParameterValue("FB L");
    auto dTimeR = apvts.getRawParameterValue ("Time R");
    auto feedbackR = apvts.getRawParameterValue("FB R");
    auto delayInput = apvts.getRawParameterValue("Delay Input");
    auto delayOutput = apvts.getRawParameterValue("Delay Output");
    auto delaySend = apvts.getRawParameterValue("Delay Send");
    
    delayButtonState = *dButtonState;
    syncButtonState = *dSyncState;
    if (syncButtonState)
    {
        *dTimeL = tempoSync.getSyncDelayTime(*dTimeL);
        *dTimeR = tempoSync.getSyncDelayTime(*dTimeR);
    }
    
    delayProcessor.setParameters(*dTimeL, *dTimeR, *feedbackL, *feedbackR, *delayInput, *verbSend);
    reverbProcessor.setParameters(*size, *damp, *wide, freeze, *verbInput, *delaySend, *dTimeL, *dTimeR);
    
    delayOutputLevel = *delayOutput;
    
    auto nMix = apvts.getRawParameterValue("Mix");
    mix = *nMix;
}

juce::AudioProcessorValueTreeState::ParameterLayout ReflectionsAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;
    
    /*------------------------------------------------------------------------*/
    parameters.push_back(std::make_unique<AudioParameterBool>  ("Verb Freeze", "Verb Freeze", false));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Size", "Size",
                                                                NormalisableRange<float> (0.0f, 1.f, 0.01f, 1.0f), 0.2f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Damping", "Damping",
                                                                NormalisableRange<float> (0.0f, 1.f, 0.01f, 1.0f), 0.2f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Width", "Width",
                                                                NormalisableRange<float> (0.0f, 1.f, 0.01f, 1.0f), 0.2f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Verb Input", "Verb Input",
                                                                NormalisableRange<float> (0.0f, 1.f, 0.01f, 1.0f), 0.2f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Verb Output", "Verb Output",
                                                                NormalisableRange<float> (0.0f, 1.f, 0.01f, 1.0f), 0.2f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Verb Send", "Verb Send",
                                                                NormalisableRange<float> (0.0f, 1.f, 0.01f, 1.0f), 0.2f));
    
    /*------------------------------------------------------------------------*/
    parameters.push_back(std::make_unique<AudioParameterBool>  ("Delay Link", "Delay Link", false));
    
    parameters.push_back(std::make_unique<AudioParameterBool>  ("Sync", "Sync", false));
    
    parameters.push_back(std::make_unique<AudioParameterFloat> ("Time L", "Delay Time L",
                                                               NormalisableRange<float> (0.0f, 2000.f, 1.f, 1.0f), 200.f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Time R", "Delay Time R",
                                                                NormalisableRange<float> (0.0f, 2000.f, 1.f, 1.0f), 200.f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("FB L", "Feedback L",
                                                                NormalisableRange<float> (0.0f, 1.0f, 0.01f, 1.0f), 0.1f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("FB R", "Feedback R",
                                                                NormalisableRange<float> (0.0f, 1.0f, 0.01f, 1.0f), 0.1f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Delay Input", "Delay Input",
                                                                NormalisableRange<float> (0.0f, 1.f, 0.01f, 1.0f), 0.2f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Delay Output", "Delay Output",
                                                                NormalisableRange<float> (0.0f, 1.f, 0.01f, 1.0f), 0.2f));
   
    parameters.push_back (std::make_unique<AudioParameterFloat>("Delay Send", "Delay Send",
                                                                NormalisableRange<float> (0.0f, 1.f, 0.01f, 1.0f), 0.1f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Mix", "Mix",
                                                                NormalisableRange<float> (0.0f, 1.f, 0.01f, 1.0f), 0.5f));
    
    /*------------------------------------------------------------------------*/
    return { parameters.begin(), parameters.end() };
}

void ReflectionsAudioProcessor::setVerbButtonState(bool state)
{
    verbButtonState = state;
    
}

bool ReflectionsAudioProcessor::getVerbButtonState()
{
    return verbButtonState;
}

void ReflectionsAudioProcessor::setDelayButtonState(bool state)
{
    delayButtonState = state;
    
}

bool ReflectionsAudioProcessor::getDelayButtonState()
{
    return delayButtonState;
}

void ReflectionsAudioProcessor::setSyncButtonState(bool state)
{
    syncButtonState = state;

}

bool ReflectionsAudioProcessor::getSyncButtonState()
{
    return syncButtonState;
}

