/*
  ==============================================================================

    ReverbProcessor.h
    Created: 8 Oct 2020 5:47:51pm
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class ReverbProcessor
{
public:
    ReverbProcessor();
    ~ReverbProcessor();
    
    void init (double sampleRate, int samplesPerBlock);
    
    AudioBuffer<float>& getVerbBuffer();
    AudioBuffer<float>& getPreviousVerbBuffer();
    void storePreviousBuffer();
    
    
    void setParameters(float size, float damping, float width, float freeze, float dryInput, float delaySend, float dTimeL, float dTimeR);
    
    void process(AudioBuffer<float>& buffer, AudioBuffer<float>& delayBuffer);
    
    void reset();
    
    
    
private:
    dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> sendLine;
    juce::Reverb reverb;
    juce::Reverb::Parameters  reverbParameters;
    
    AudioBuffer<float> verbBuffer;
    AudioBuffer<float> previousVerbBuffer;
    
    Atomic<float> dryInputLevel     { 0.2f };
    Atomic<float> delaySendLevel    { 0.2f };
    Atomic<float>   delayTimeL      { 0.0f };
    Atomic<float>   delayTimeR      { 0.0f };
    
    juce::SmoothedValue<float, ValueSmoothingTypes::Linear> delaySmoothedL;
    juce::SmoothedValue<float, ValueSmoothingTypes::Linear> delaySmoothedR;
    
   
    
    
    
    double mSampleRate;
    
    
};
