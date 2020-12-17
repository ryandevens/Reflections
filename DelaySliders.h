/*
  ==============================================================================

    DelaySliders.h
    Created: 10 Oct 2020 6:07:47pm
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "DelayFeel.h"
#include "TempoSync.h"

//==============================================================================
/*
*/
class DelaySliders  : public juce::Component,
public juce::Button::Listener, public juce::Slider::Listener
{
public:
    DelaySliders(ReflectionsAudioProcessor& audioProcessor);
    ~DelaySliders() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void buttonClicked(Button* mButton) override;
    void sliderValueChanged(Slider* slider) override;
    
    bool paintConnected = false;
private:
    DelayFeel delayFeel;
    TempoSync tempoSync;
    ReflectionsAudioProcessor& mProcessor;
    GlowEffect glowEffect;
    
    
    std::unique_ptr<juce::TextButton> button;
    std::unique_ptr<juce::TextButton> syncButton;
    
    std::unique_ptr<juce::ToggleButton> fbButton;
    
    std::unique_ptr<juce::Slider> delaySliderL;
    std::unique_ptr<juce::Slider> feedbackSliderL;
    std::unique_ptr<juce::Slider> delaySliderR;
    std::unique_ptr<juce::Slider> feedbackSliderR;
    std::unique_ptr<juce::Slider> delayInputSlider;
    std::unique_ptr<juce::Slider> delayOutputSlider;
    std::unique_ptr<juce::Slider> delaySendSlider;
    
    
    std::unique_ptr<Label> syncLabelL;
    std::unique_ptr<Label> syncLabelR;
    std::unique_ptr<Label> fbButtonLabel;
    std::unique_ptr<Label> leftLabel;
    std::unique_ptr<Label> rightLabel;
    std::unique_ptr<Label> fbLabelL;
    std::unique_ptr<Label> fbLabelR;
    std::unique_ptr<Label> delayInputLabel;
    std::unique_ptr<Label> delayOutputLabel;
    std::unique_ptr<Label> delaySendLabel;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> syncButtonAttachment;
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> delayAttachmentL;
    std::unique_ptr<Attachment> feedbackAttachmentL;
    std::unique_ptr<Attachment> delayAttachmentR;
    std::unique_ptr<Attachment> feedbackAttachmentR;
    std::unique_ptr<Attachment> delayInputAttachment;
    std::unique_ptr<Attachment> delayOutputAttachment;
    std::unique_ptr<Attachment> delaySendAttachment;
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelaySliders)
};
