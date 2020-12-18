/*
  ==============================================================================

    ReverbProcessor.cpp
    Created: 8 Oct 2020 5:47:51pm
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#include "ReverbProcessor.h"

ReverbProcessor::ReverbProcessor() : sendLine(1000000), verbBuffer(2, 4096), previousVerbBuffer(2, 4096)
{
    
};

ReverbProcessor::~ReverbProcessor(){};

void ReverbProcessor::init(double sampleRate, int samplesPerBlock)
{
    sendLine.prepare({ mSampleRate, (uint32) samplesPerBlock, 2 });
    verbBuffer.setSize(2, samplesPerBlock);
    previousVerbBuffer.setSize(2, samplesPerBlock);
    
    verbBuffer.clear();
    previousVerbBuffer.clear();
    
    mSampleRate = sampleRate;
    reverb.setSampleRate(mSampleRate);
    
    
    delaySmoothedL.reset(mSampleRate, 1.0);
    delaySmoothedR.reset(mSampleRate, 1.0);
    
    reverbParameters.wetLevel = 1.0f;
}

AudioBuffer<float>& ReverbProcessor::getVerbBuffer()
{
    return verbBuffer;
}

AudioBuffer<float>& ReverbProcessor::getPreviousVerbBuffer()
{
    auto previousVerbBufferWrite = previousVerbBuffer.getArrayOfWritePointers();
    
    for (int i = 0; i < previousVerbBuffer.getNumSamples(); ++i)
    {
        auto dTimeL = delaySmoothedL.getNextValue();
        auto dTimeR = delaySmoothedR.getNextValue();
        
        for (int channel = 0; channel < 2; ++channel)
        {
            float wetSample;
            if (channel == 0)
            {
                wetSample = sendLine.popSample(channel, dTimeL, true);
                previousVerbBufferWrite[channel][i] = wetSample;
            }
            else
            {
                wetSample = sendLine.popSample(channel, dTimeR, true);
                previousVerbBufferWrite[channel][i] = wetSample;
            }
        }
        
    }
    
    return previousVerbBuffer;
}

void ReverbProcessor::storePreviousBuffer()
{
    
    //previousVerbBuffer.makeCopyOf(verbBuffer);
}

void ReverbProcessor::setParameters(float size, float damping, float width, float freeze, float dryInput, float delaySend, float dTimeL, float dTimeR)
{
    reverbParameters.roomSize = size;
    reverbParameters.damping = damping;
    reverbParameters.width = width;
    reverbParameters.freezeMode = freeze;
    reverb.setParameters(reverbParameters);
    
    dryInputLevel = dryInput;
    delaySendLevel = delaySend;
    
    delayTimeL = dTimeL;
    delayTimeR = dTimeR;
    
    delaySmoothedL.setTargetValue(delayTimeL.get() * mSampleRate / 1000); // delay in samples
    delaySmoothedR.setTargetValue(delayTimeR.get() * mSampleRate / 1000);
}
/*
    This required some gross code.  I have to copy the processorBuffer to the verbBuffer.
    This is because the built in 'processStereo()' function takes write pointers in directly
    It is designed to write each sample to the same buffer directly and prohibits indirection
    (or so it would seem)
*/
void ReverbProcessor::process(AudioBuffer<float>& buffer, AudioBuffer<float>& delayBuffer)
{
    verbBuffer.makeCopyOf(buffer);
    verbBuffer.applyGain(dryInputLevel.get());
        // add sample values from our delayBuffer to our verbBuffer so they get sent through the reverb
    for(int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        verbBuffer.addFrom(channel, 0,  delayBuffer, channel, 0, verbBuffer.getNumSamples(), delaySendLevel.get());
    }
    if (verbBuffer.getNumChannels() <= 1)
    {
        reverb.processMono(verbBuffer.getWritePointer(0), buffer.getNumSamples());
    }
    else
    {
//        float* const left = verbBuffer.getWritePointer(0);
//        float* const right = verbBuffer.getWritePointer(1);
        reverb.processStereo(verbBuffer.getWritePointer(0), verbBuffer.getWritePointer(1), verbBuffer.getNumSamples());
    }
    
    
    auto verbBufferRead = verbBuffer.getArrayOfReadPointers();
    
    for (int i = 0; i < verbBuffer.getNumSamples(); ++i)
    {
        for (int channel = 0; channel < verbBuffer.getNumChannels(); ++channel)
        {
            float verbSample = verbBufferRead[channel][i];
            
            if (channel == 0)
            {
                sendLine.pushSample(channel, verbSample);
            }
            else
            {
                sendLine.pushSample(channel, verbSample);
            }
        }
     }
}


void ReverbProcessor::reset()
{
    reverb.reset();
}
