/*
  ==============================================================================

    DelayProcessor.h
    Created: 8 Oct 2020 4:57:06pm
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class DelayProcessor
{
public:
    DelayProcessor();
    ~DelayProcessor();
    
    void init(double sampleRate, int samplesPerBlock);
    
    AudioBuffer<float>& getDelayBuffer();
    AudioBuffer<float>& getPreviousDelayBuffer();
    
    void storePreviousBuffer();
    
    void setParameters(float dTimeL, float dTimeR, float fbL, float fbR, float dryInput, float verbSend);
    
    void reset();
    
    void process(AudioBuffer<float>& buffer, AudioBuffer<float>& verbBuffer);
    
    bool getCurrentPosition (AudioPlayHead::CurrentPositionInfo& result);
    
    
    float getDelayL();
    float getDelayR();
    float getFeedbackL();
    float getFeedbackR();
    
    Atomic<float>   delayTimeL               { 200.0f };
    Atomic<float>   delayTimeR               { 200.0f };
    Atomic<float>   feedbackLevelL           { 0.2f };
    Atomic<float>   feedbackLevelR           { 0.2f };
    Atomic<float>   dryInputLevel            { 0.2f };
    Atomic<float>   verbSendLevel            { 0.2f };
   
private:
    dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayLine;
    dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> sendLine;
    
    AudioBuffer<float> delayBuffer;
    AudioBuffer<float> previousDelayBuffer;
    
    double mSampleRate;

    float verbFeedbackCoeff = 0.2f;
    
    juce::SmoothedValue<float, ValueSmoothingTypes::Linear> delaySmoothedL;
    juce::SmoothedValue<float, ValueSmoothingTypes::Linear> delaySmoothedR;
    std::vector<float> lastOutput { 2 };
    
    
    
};
