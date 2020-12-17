/*
  ==============================================================================

    TempoSync.cpp
    Created: 22 Oct 2020 7:44:03am
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#include "TempoSync.h"

TempoSync::TempoSync(){}

TempoSync::~TempoSync(){}

void TempoSync::setTempo(double tempo, int timeSigNum, int timeSigDenom)
{
    bpm = tempo;
    quarterNote = 60000 / bpm;
    wholeNote = quarterNote * 4;
    halfNote = quarterNote * 2;
    eighthNote = quarterNote / 2;
    sixteenthNote = quarterNote / 4;
}

juce::String TempoSync::getSyncString(float sliderValue)
{

    if (sliderValue <= 40)
        return "1/16";

    if (sliderValue > 400 && sliderValue <= 80)
        return "1/8";

    if (sliderValue > 800 && sliderValue <= 120)
        return "1/4";

    if (sliderValue > 1200 && sliderValue <= 160)
        return "1/2";

    if (sliderValue > 1600 && sliderValue <= 200)
        return "1/1";

}



float TempoSync::getSyncDelayTime(float delayTime)
{
    if (delayTime <= 40)
        return sixteenthNote;

    if (delayTime > 40 && delayTime <= 80)
        return eighthNote;

    if (delayTime > 800 && delayTime <= 120)
        return quarterNote;

    if (delayTime > 1200 && delayTime <= 160)
        return halfNote;
//
    if (delayTime > 1600) // max delay is one whole no
        return wholeNote;
}
//
//
//
//


