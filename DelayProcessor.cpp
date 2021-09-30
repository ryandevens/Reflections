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
    auto previousDelayBufferWrite = previousDelayBuffer.getArrayOfWritePointers();
    
    auto dTimeL = delaySmoothedL.getCurrentValue();
    auto dTimeR = delaySmoothedR.getCurrentValue();
    
//    delaySmoothedL.setTargetValue(dTimeL * mSampleRate / 1000.0);
//    delaySmoothedR.setTargetValue(dTimeR * mSampleRate / 1000.0);
    
    for (int i = 0; i < previousDelayBuffer.getNumSamples(); ++i)
    {
        
        
        for (int channel = 0; channel < 2; ++channel)
        {
            float wetSample;
            if (channel == 0)
            {
                wetSample = sendLine.popSample(channel, dTimeL, true);
                previousDelayBufferWrite[channel][i] = wetSample;
            }
            else
            {
                wetSample = sendLine.popSample(channel, dTimeR, true);
                previousDelayBufferWrite[channel][i] = wetSample;
            }
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
    return delayTimeL.get();
}

float DelayProcessor::getDelayR()
{
    return delayTimeR.get();
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
    
    int totalNumInputChannels = buffer.getNumChannels();
    
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        auto dTimeL = delaySmoothedL.getNextValue();
        auto dTimeR = delaySmoothedR.getNextValue();
        auto fbL = feedbackLevelL.get();
        auto fbR = feedbackLevelR.get();
        
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            float drySample = processorBufferRead[channel][i];
            drySample *= dryInputLevel.get();
            
            float verbSample = verbBufferRead[channel][i];
            verbSample *= verbSendLevel.get();
            
    
            if (channel == 0)
            {
                delayLine.pushSample(channel, drySample + verbSample + lastOutput[channel]);
                
                auto wetSample = delayLine.popSample(channel, dTimeL, true);
                
                delayBufferWrite[channel][i] = wetSample;
                
                sendLine.pushSample(channel, wetSample);
                
                lastOutput[channel] = wetSample * fbL;
            }
            else
            {
                delayLine.pushSample(channel, drySample + verbSample + lastOutput[channel]);
                
                auto wetSample = delayLine.popSample(channel, dTimeR, true);
                
                delayBufferWrite[channel][i] = wetSample;
                
                sendLine.pushSample(channel, wetSample);
                
                lastOutput[channel] = wetSample * fbR;
            }
            
        }
        
    }
}



