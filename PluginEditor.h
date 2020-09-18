/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "DelayFeel.h"

//==============================================================================
/**
*/
class ReflectionsAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                         public juce::Button::Listener
{
public:
    ReflectionsAudioProcessorEditor (ReflectionsAudioProcessor&);
    ~ReflectionsAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void buttonClicked(Button* mButton) override;

private:
    DelayFeel delayFeel;
    
    std::unique_ptr<juce::ToggleButton> button;
    
    std::unique_ptr<juce::Slider>   mixSlider;
    std::unique_ptr<juce::Slider>   sizeSlider;
    std::unique_ptr<juce::Slider>   dampSlider;
    std::unique_ptr<juce::Slider>   widthSlider;
    
    std::unique_ptr<Label>          buttonLabel;
    std::unique_ptr<Label>          mixLabel;
    std::unique_ptr<Label>          sizeLabel;
    std::unique_ptr<Label>          dampLabel;
    std::unique_ptr<Label>          widthLabel;
    
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment>     mixAttachment;
    std::unique_ptr<Attachment>     sizeAttachment;
    std::unique_ptr<Attachment>     dampAttachment;
    std::unique_ptr<Attachment>     widthAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonAttachment;
    
    bool paintFrozen = false;
    
    ReflectionsAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReflectionsAudioProcessorEditor)
};
