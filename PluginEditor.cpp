/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ReflectionsAudioProcessorEditor::ReflectionsAudioProcessorEditor (ReflectionsAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(delaySliders);
    delaySliders.setTopLeftPosition(10, 40);
    addAndMakeVisible(reverbSliders);
    reverbSliders.setTopLeftPosition(360, 40);
    
    setLookAndFeel(&mixFeel);
    
    mixSlider = std::make_unique<Slider>(Slider::SliderStyle::LinearBarVertical, Slider::TextBoxBelow);
    mixSlider->setBounds(302, 65, 40, 240);
    addAndMakeVisible (mixSlider.get());
    
    mixLabel = std::make_unique<Label>("", "Mix");
    addAndMakeVisible (mixLabel.get());
    
    mixLabel->attachToComponent (mixSlider.get(), false);
    mixLabel->setJustificationType (Justification::centred);
    
    mixAttachment = std::make_unique<Attachment>(audioProcessor.apvts, "Mix", *mixSlider);
    
    
    nameLabel = std::make_unique<Label>("", "REFLECTIONS");
    addAndMakeVisible (nameLabel.get());
    nameLabel->setBounds(238, 13, 200, 27);
    nameLabel->setLookAndFeel(&nameFeel);
    
    setSize (645, 325);
}

ReflectionsAudioProcessorEditor::~ReflectionsAudioProcessorEditor()
{
}

//==============================================================================
void ReflectionsAudioProcessorEditor::paint (juce::Graphics& g)
{
    
    g.setColour(Colours::black);
    g.fillAll();
    
    g.setColour(Colours::lightgrey.brighter());
    auto bounds = getLocalBounds().toFloat();
    g.drawRoundedRectangle(bounds.reduced(5), 5.f, 1.0f);

}

void ReflectionsAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}


