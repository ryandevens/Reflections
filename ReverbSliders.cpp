/*
  ==============================================================================

    ReverbSliders.cpp
    Created: 13 Oct 2020 4:08:00pm
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ReverbSliders.h"

//==============================================================================
ReverbSliders::ReverbSliders(ReflectionsAudioProcessor& audioProcessor) : mProcessor(audioProcessor)
{
    setLookAndFeel(&verbFeel);
    /*--------------- Sliders & Button -----------------*/
    verbInputSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextBoxBelow);
    verbInputSlider->setBounds(25, 200, 70, 70);
    addAndMakeVisible (verbInputSlider.get());
    
    verbOutputSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextBoxBelow);
    verbOutputSlider->setBounds(100, 200, 70, 70);
    addAndMakeVisible (verbOutputSlider.get());
    
    verbSendSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextBoxBelow);
    verbSendSlider->setBounds(175, 200, 70, 70);
    addAndMakeVisible (verbSendSlider.get());
    
    sizeSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextBoxBelow);
    sizeSlider->setBounds(100, 25, 70, 70);
    addAndMakeVisible (sizeSlider.get());
    
    dampSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextBoxBelow);
    //dampSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 25, 2); Using this enlarges the slider for some unknown reason
    dampSlider->setBounds(25, 100, 70, 70);
    addAndMakeVisible (dampSlider.get());
    
    widthSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextBoxBelow);
    widthSlider->setBounds(175, 100, 70, 70);
    addAndMakeVisible (widthSlider.get());
    
    button = std::make_unique<TextButton>("Freeze");
    button->setBounds(110, 120, 50, 25);
    button->setClickingTogglesState(true);
    button->addListener(this);
    addAndMakeVisible(button.get());
    
    /*----------------- Labels ----------------*/
    verbInputLabel = std::make_unique<Label>("", "Dry Input");
    addAndMakeVisible (verbInputLabel.get());
    verbInputLabel->attachToComponent (verbInputSlider.get(), false);
    verbInputLabel->setJustificationType (Justification::centred);
    
    verbOutputLabel = std::make_unique<Label>("", "Verb Output");
    addAndMakeVisible (verbOutputLabel.get());
    verbOutputLabel->attachToComponent (verbOutputSlider.get(), false);
    verbOutputLabel->setJustificationType (Justification::centred);
    
    verbSendLabel = std::make_unique<Label>("", "Verb Send");
    addAndMakeVisible (verbSendLabel.get());
    verbSendLabel->attachToComponent (verbSendSlider.get(), false);
    verbSendLabel->setJustificationType (Justification::centred);
    
    sizeLabel = std::make_unique<Label>("", "Room Size");
    addAndMakeVisible (sizeLabel.get());
    sizeLabel->attachToComponent (sizeSlider.get(), false);
    sizeLabel->setJustificationType (Justification::centred);
    
    dampLabel = std::make_unique<Label>("", "Damping");
    addAndMakeVisible (dampLabel.get());
    dampLabel->attachToComponent (dampSlider.get(), false);
    dampLabel->setJustificationType (Justification::centred);
    
    widthLabel = std::make_unique<Label>("", "Width");
    addAndMakeVisible (widthLabel.get());
    widthLabel->attachToComponent (widthSlider.get(), false);
    widthLabel->setJustificationType (Justification::centred);
    
//    buttonLabel = std::make_unique<Label>("", "Freeze");
//    buttonLabel->setBounds(113, 110, 50, 15);
//    addAndMakeVisible(buttonLabel.get());
    
    /*---------------- Attachments --------------*/
    sizeAttachment = std::make_unique<Attachment>(mProcessor.apvts, "Size", *sizeSlider);
    dampAttachment = std::make_unique<Attachment>(mProcessor.apvts, "Damping", *dampSlider);
    widthAttachment = std::make_unique<Attachment>(mProcessor.apvts, "Width", *widthSlider);
    
    verbInputAttachment = std::make_unique<Attachment>(mProcessor.apvts, "Verb Input", *verbInputSlider);
    verbOutputAttachment = std::make_unique<Attachment>(mProcessor.apvts, "Verb Output", *verbOutputSlider);
    verbSendAttachment = std::make_unique<Attachment>(mProcessor.apvts, "Verb Send", *verbSendSlider);
    
    buttonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>
    (mProcessor.apvts, "Verb Freeze", *button);
    
    auto& lnf = getLookAndFeel();
    lnf.setColour (Slider::textBoxTextColourId, Colours::skyblue.darker());
    lnf.setColour (Slider::textBoxHighlightColourId, Colours::whitesmoke);
    lnf.setColour (Slider::textBoxOutlineColourId, Colours::transparentWhite);
    
    sendLookAndFeelChange();
    
    setSize (275, 275);

}

ReverbSliders::~ReverbSliders()
{
}

void ReverbSliders::paint (juce::Graphics& g)
{

    
    g.setColour(Colours::black);

    g.fillAll();
    
    g.setColour (juce::Colours::skyblue);
    
    
    juce::Path sectionPath;
    sectionPath.startNewSubPath(juce::Point<float> (10, 175));
    sectionPath.lineTo(juce::Point<float> (265, 175));
    sectionPath.lineTo(juce::Point<float> (265, 176));
    sectionPath.lineTo(juce::Point<float> (10, 176));
    sectionPath.closeSubPath();

    g.fillPath(sectionPath);

    auto bounds = getLocalBounds().toFloat();
    g.drawRoundedRectangle(bounds.reduced(5), 5.f, 1.5f);
    
}

void ReverbSliders::resized()
{
   
}

void ReverbSliders::buttonClicked(Button* mButton)
{
    if (mButton == button.get())
    {
        if (mProcessor.getVerbButtonState() == false)
        {
            mProcessor.setVerbButtonState(true);
            
            dampSlider->setColour(Slider::rotarySliderFillColourId, Colours::grey.brighter());
            sizeSlider->setColour(Slider::rotarySliderFillColourId, Colours::grey.brighter());
            
            mProcessor.update();
        } else
        {
            mProcessor.setVerbButtonState(false);
            
            dampSlider->removeColour(Slider::rotarySliderFillColourId);
            sizeSlider->removeColour(Slider::rotarySliderFillColourId);
            
            
            mProcessor.update();
        }
    }
}
