/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "DelaySliders.h"
#include "ReverbSliders.h"
#include "MixFeel.h"
#include "NameFeel.h"


//==============================================================================
/**
*/
class ReflectionsAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ReflectionsAudioProcessorEditor (ReflectionsAudioProcessor&);
    ~ReflectionsAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    
    
private:
    ReflectionsAudioProcessor& audioProcessor;
    MixFeel mixFeel;
    NameFeel nameFeel;
    
    DelaySliders  delaySliders   { audioProcessor };
    ReverbSliders reverbSliders  { audioProcessor };
    
    
    std::unique_ptr<juce::Slider>   mixSlider;
    std::unique_ptr<Label>          mixLabel;
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment>     mixAttachment;
    
    std::unique_ptr<Label>          nameLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReflectionsAudioProcessorEditor)
};
