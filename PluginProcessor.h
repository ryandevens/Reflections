/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DelayProcessor.h"
#include "ReverbProcessor.h"
#include "TempoSync.h"
//#include "Analyser.h"

//==============================================================================
/**
*/
class ReflectionsAudioProcessor  : public AudioProcessor,
public ValueTree::Listener
{
public:
    //==============================================================================
    ReflectionsAudioProcessor();
    ~ReflectionsAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    void setVerbButtonState(bool state);
    bool getVerbButtonState();
    
    void setDelayButtonState(bool state);
    bool getDelayButtonState();
    
    void setSyncButtonState(bool state);
    bool getSyncButtonState();
    
    AudioProcessorValueTreeState apvts;
    AudioProcessorValueTreeState::ParameterLayout createParameters();
    
    void update();
    
    void setProcessingState(bool isProcessing);

private:
    ReverbProcessor     reverbProcessor;
    DelayProcessor      delayProcessor;
    
    AudioPlayHead*                     playHead;
    AudioPlayHead::CurrentPositionInfo currentPos;
    TempoSync     tempoSync;
    
    //Analyser<float>   outputAnalyser;
    
    Atomic<bool> isProcessing { false };
    
    Atomic<float> delayOutputLevel  { 0.0f };
    Atomic<float> verbOutputLevel   { 0.0f };
    Atomic<float> mix   { 0.0f };
    
    bool verbButtonState = false;
    bool delayButtonState = false;
    bool syncButtonState = false;
    bool isFirstTime = true;
    int safetyCounter = 5;
    
    bool mustUpdateProcessing { false };
    
    void valueTreePropertyChanged(ValueTree& tree, const Identifier& property) override
    {
        mustUpdateProcessing = true;
    }
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReflectionsAudioProcessor)
};
