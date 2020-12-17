/*
  ==============================================================================

    TempoSync.h
    Created: 22 Oct 2020 7:44:02am
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class TempoSync
{
public:
    TempoSync();
    ~TempoSync();
    
    void setTempo(double tempo, int timeSigNum, int timeSigDenom);
    
    
    juce::String getSyncString(float sliderValue);
    
    float getSyncDelayTime(float delayTime);
    
    
    
private:
    float wholeNote, halfNote, quarterNote, eighthNote, sixteenthNote;
    double bpm = 120;
};

