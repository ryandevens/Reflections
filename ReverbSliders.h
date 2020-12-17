/*
  ==============================================================================

    ReverbSliders.h
    Created: 13 Oct 2020 4:08:00pm
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "VerbFeel.h"
//==============================================================================
/*
*/
class ReverbSliders  : public juce::Component,
public juce::Button::Listener
{
public:
    ReverbSliders(ReflectionsAudioProcessor& audioProcessor);
    ~ReverbSliders() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void buttonClicked(Button* mButton) override;

private:
    VerbFeel verbFeel;
    std::unique_ptr<juce::TextButton> button;
    
    std::unique_ptr<juce::Slider>   sizeSlider;
    std::unique_ptr<juce::Slider>   dampSlider;
    std::unique_ptr<juce::Slider>   widthSlider;
    
    std::unique_ptr<juce::Slider>   verbInputSlider;
    std::unique_ptr<juce::Slider>   verbOutputSlider;
    std::unique_ptr<juce::Slider>   verbSendSlider;
    
    std::unique_ptr<Label>          buttonLabel;
    std::unique_ptr<Label>          sizeLabel;
    std::unique_ptr<Label>          dampLabel;
    std::unique_ptr<Label>          widthLabel;
    
    std::unique_ptr<Label>          verbInputLabel;
    std::unique_ptr<Label>          verbOutputLabel;
    std::unique_ptr<Label>          verbSendLabel;
    
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    std::unique_ptr<Attachment>     sizeAttachment;
    std::unique_ptr<Attachment>     dampAttachment;
    std::unique_ptr<Attachment>     widthAttachment;
    
    std::unique_ptr<Attachment>     verbInputAttachment;
    std::unique_ptr<Attachment>     verbOutputAttachment;
    std::unique_ptr<Attachment>     verbSendAttachment;

    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonAttachment;
    
    bool paintFrozen = false;
    
    ReflectionsAudioProcessor& mProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbSliders)
};
