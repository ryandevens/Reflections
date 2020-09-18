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
    mSampleRate = sampleRate;
    reverb.setSampleRate(mSampleRate);
    update();
}

void ReflectionsAudioProcessor::releaseResources()
{
    reverb.reset();
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

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    if (mustUpdateProcessing)
        update();
    
    float* const left = buffer.getWritePointer(0);
    float* const right = buffer.getWritePointer(1);
    
    reverb.processStereo(left, right, buffer.getNumSamples());
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

}

void ReflectionsAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
 
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
    auto bState = apvts.getRawParameterValue("button");
    auto mix = apvts.getRawParameterValue("Mix");
    auto dry = 1 - *mix;
    auto size = apvts.getRawParameterValue("Size");
    auto damp = apvts.getRawParameterValue("Damping");
    auto wide = apvts.getRawParameterValue("Width");
    auto freeze = 0.0f;
    
    if (*bState)
        freeze = 1.0f;
    else
        freeze = 0.0f;
    
    
    
    reverbParameters.wetLevel = *mix;
    reverbParameters.dryLevel = dry;
    reverbParameters.roomSize = *size;
    reverbParameters.damping = *damp;
    reverbParameters.width = *wide;
    reverbParameters.freezeMode = freeze;
    reverb.setParameters(reverbParameters);
}

juce::AudioProcessorValueTreeState::ParameterLayout ReflectionsAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;
    
    parameters.push_back(std::make_unique<AudioParameterBool>("button", "button", false));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Mix", "Mix",
                        NormalisableRange<float> (0.0f, 1.f, 0.01f, 1.0f), 0.0f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Size", "Size",
                        NormalisableRange<float> (0.0f, 1.f, 0.01f, 1.0f), 0.0f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Damping", "Damping",
                        NormalisableRange<float> (0.0f, 1.f, 0.01f, 1.0f), 0.0f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Width", "Width",
                        NormalisableRange<float> (0.0f, 1.f, 0.01f, 1.0f), 0.0f));
    
    return { parameters.begin(), parameters.end() };
}

void ReflectionsAudioProcessor::setButtonState(bool state)
{
    buttonState = state;
    
}

bool ReflectionsAudioProcessor::getButtonState()
{
    return buttonState;
}
