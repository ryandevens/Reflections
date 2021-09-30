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
#include "VerbFeel.h"


//==============================================================================
/**
*/
class ReflectionsAudioProcessorEditor  : public juce::AudioProcessorEditor,
public Button::Listener
{
public:
    ReflectionsAudioProcessorEditor (ReflectionsAudioProcessor&);
    ~ReflectionsAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void buttonClicked(Button* b) override;
    
    
private:
    ReflectionsAudioProcessor& audioProcessor;
    MixFeel mixFeel;
    NameFeel nameFeel;
    VerbFeel verbFeel;
    
    DelaySliders  delaySliders   { audioProcessor };
    ReverbSliders reverbSliders  { audioProcessor };
    
    std::unique_ptr<TextButton>     processingButton;
    std::unique_ptr<juce::Slider>   mixSlider;
    std::unique_ptr<Label>          mixLabel;
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment>     mixAttachment;
    
    std::unique_ptr<Label>          nameLabel;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReflectionsAudioProcessorEditor)
};
