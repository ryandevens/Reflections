/*
  ==============================================================================

    DelayProcessor.cpp
    Created: 8 Oct 2020 4:57:06pm
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#include "DelayProcessor.h"

DelayProcessor::DelayProcessor() : delayLine(1000000), sendLine(1000000), delayBuffer(2, 4096), previousDelayBuffer(2, 4096)
{

};

DelayProcessor::~DelayProcessor(){};

void DelayProcessor::init(double sampleRate, int samplesPerBlock)
{
    delayBuffer.setSize(2, samplesPerBlock);
    previousDelayBuffer.setSize(2, samplesPerBlock);
    delayBuffer.clear();
    previousDelayBuffer.clear();
    
    mSampleRate = sampleRate;
    delayLine.prepare({ mSampleRate, (uint32) samplesPerBlock, 2 });
    sendLine.prepare({ mSampleRate, (uint32) samplesPerBlock, 2 });
    
    delayLine.reset();
    sendLine.reset();
    
    delaySmoothedL.reset(mSampleRate, 0.5);
    delaySmoothedR.reset(mSampleRate, 0.5);
}

AudioBuffer<float>& DelayProcessor::getDelayBuffer()
{
    return delayBuffer;
}

AudioBuffer<float>& DelayProcessor::getPreviousDelayBuffer()
{
    previousDelayBuffer.clear();
    auto previousDelayBufferWrite = previousDelayBuffer.getArrayOfWritePointers();
    
    Array<float> dTimes(delaySmoothedL.getCurrentValue(), delaySmoothedR.getCurrentValue());
    
    for (int i = 0; i < previousDelayBuffer.getNumSamples(); ++i)
    {
        
        for (int channel = 0; channel < dTimes.size(); ++channel)
        {
            float wetSample= sendLine.popSample(channel, dTimes[channel], true);
            previousDelayBufferWrite[channel][i] = wetSample;
        }
    }
    
    return previousDelayBuffer;
}

void DelayProcessor::storePreviousBuffer()
{
    

    //previousDelayBuffer.makeCopyOf(delayBuffer);
}

void DelayProcessor::setParameters(float dTimeL, float dTimeR, float fbL, float fbR, float inputLevel, float sendLevel)
{
    delaySmoothedL.setTargetValue(dTimeL * mSampleRate / 1000.0); // convert to samples
    delaySmoothedR.setTargetValue(dTimeR * mSampleRate / 1000.0); 
    feedbackLevelL = fbL;
    feedbackLevelR = fbR;
    dryInputLevel = inputLevel;
    verbSendLevel = sendLevel;

}

void DelayProcessor::reset()
{
    delayLine.reset();
}

float DelayProcessor::getDelayL()
{
    return delaySmoothedL.getCurrentValue();
}

float DelayProcessor::getDelayR()
{
    return delaySmoothedR.getCurrentValue();
}

float DelayProcessor::getFeedbackL()
{
    return feedbackLevelL.get();
}

float DelayProcessor::getFeedbackR()
{
    return feedbackLevelR.get();
}

void DelayProcessor::process(AudioBuffer<float>& buffer, AudioBuffer<float>& verbBuffer)
{
    auto processorBufferRead = buffer.getArrayOfReadPointers();
    auto verbBufferRead = verbBuffer.getArrayOfReadPointers();
    delayBuffer.clear();
    auto delayBufferWrite = delayBuffer.getArrayOfWritePointers();
    
    int numInputChannels = buffer.getNumChannels();

    Array<float> fbArray(feedbackLevelL.get(), feedbackLevelR.get());

    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        Array<float> dTimes(delaySmoothedL.getNextValue(), delaySmoothedR.getNextValue());

        for (int channel = 0; channel < numInputChannels; ++channel)
        {
            float drySample = processorBufferRead[channel][i];
            drySample *= dryInputLevel.get();
            
            float verbSample = verbBufferRead[channel][i];
            verbSample *= verbSendLevel.get();
            
            delayLine.pushSample(channel, drySample + verbSample + lastOutput[channel]);

            auto wetSample = delayLine.popSample(channel, dTimes[channel], true);

            delayBufferWrite[channel][i] = wetSample;

            sendLine.pushSample(channel, wetSample);

            lastOutput[channel] = wetSample * fbArray[channel];
        }
        
    }
}



