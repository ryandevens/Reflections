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
    setLookAndFeel(&delayFeel);
    
    /*--------------- Sliders & Button -----------------*/
    mixSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextBoxBelow);
    mixSlider->setBounds(150, 250, 100, 100);
    addAndMakeVisible (mixSlider.get());
    
    sizeSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextBoxBelow);
    sizeSlider->setBounds(150, 50, 100, 100);
    addAndMakeVisible (sizeSlider.get());
    
    dampSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextBoxBelow);
    dampSlider->setBounds(50, 150, 100, 100);
    addAndMakeVisible (dampSlider.get());
    
    widthSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextBoxBelow);
    widthSlider->setBounds(250, 150, 100, 100);
    addAndMakeVisible (widthSlider.get());
    
    button = std::make_unique<ToggleButton>();
    button->setBounds(150, 400, 100, 25);
    button->addListener(this);
    addAndMakeVisible(button.get());
    
    /*----------------- Labels ----------------*/
    mixLabel = std::make_unique<Label>("", "Wet Mix");
    addAndMakeVisible (mixLabel.get());
    mixLabel->attachToComponent (mixSlider.get(), false);
    mixLabel->setJustificationType (Justification::centred);
    
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
    
    buttonLabel = std::make_unique<Label>("", "Freeze");
    buttonLabel->setBounds(170, 375, 70, 15);
    addAndMakeVisible(buttonLabel.get());
    
    /*---------------- Attachments --------------*/
    mixAttachment = std::make_unique<Attachment>(audioProcessor.apvts, "Mix", *mixSlider);
    sizeAttachment = std::make_unique<Attachment>(audioProcessor.apvts, "Size", *sizeSlider);
    dampAttachment = std::make_unique<Attachment>(audioProcessor.apvts, "Damping", *dampSlider);
    widthAttachment = std::make_unique<Attachment>(audioProcessor.apvts, "Width", *widthSlider);
    
    buttonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>
    (audioProcessor.apvts, "button", *button);
    
    auto& lnf = getLookAndFeel();
    lnf.setColour (Slider::textBoxTextColourId, Colours::skyblue.darker());
    lnf.setColour (Slider::textBoxHighlightColourId, Colours::whitesmoke);
    lnf.setColour (Slider::textBoxOutlineColourId, Colours::transparentWhite);
    
    sendLookAndFeelChange();
    
    setSize (400, 500);
}

ReflectionsAudioProcessorEditor::~ReflectionsAudioProcessorEditor()
{
}

//==============================================================================
void ReflectionsAudioProcessorEditor::paint (juce::Graphics& g)
{
    auto black = juce::Colours::black;
    auto white = juce::Colours::dimgrey;
    auto bounds = getLocalBounds().toFloat();
    Point<float> centre(bounds.getCentre().toFloat());
    /* dummy value because gradient is radial */
    Point<float> right(bounds.getTopRight().toFloat());
    
    g.setColour(black);
    juce::ColourGradient fillGradient(white.darker(0.2), centre, white.darker(0.6), right, true);
    g.setGradientFill(fillGradient);
    g.fillAll();
    
    g.setColour (juce::Colours::orange);
    
    juce::Path path;
    path.startNewSubPath (juce::Point<float> (100, 284));
    path.lineTo (juce::Point<float> (200, 190));
    path.lineTo (juce::Point<float> (101, 285));
    path.closeSubPath();
    
    juce::Path path2;
    path2.startNewSubPath(juce::Point<float> (301, 284));
    path2.lineTo(juce::Point<float> (200, 190));
    path2.lineTo(juce::Point<float> (300, 285));
    path2.closeSubPath();
    
    juce::Path path3;
    path3.startNewSubPath(juce::Point<float> (300, 99));
    path3.lineTo(juce::Point<float> (200, 190));
    path3.lineTo(juce::Point<float> (301, 100));
    path3.closeSubPath();
    
    juce::Path path4;
    path4.startNewSubPath(juce::Point<float> (100, 99));
    path4.lineTo(juce::Point<float> (200, 190));
    path4.lineTo(juce::Point<float> (99, 100));
    path4.closeSubPath();
    
    juce::Path curve;
    juce::PathStrokeType stroke(0.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);
    
    for (int i = 0; i < 10; i++)
    {
        float destX = i * 20;
        float destY = i * 5;
        curve.quadraticTo(50.f, 50.f, destX, destY);
        g.strokePath(curve, stroke);
    }
    
    //
    
    g.fillPath(path);
    g.fillPath(path2);
    g.fillPath(path3);
    g.fillPath(path4);
    
}

void ReflectionsAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void ReflectionsAudioProcessorEditor::buttonClicked(Button* mButton)
{
    if (mButton == button.get())
    {
        if (audioProcessor.getButtonState() == false)
        {
            audioProcessor.setButtonState(true);
            
            dampSlider->setColour(Slider::rotarySliderFillColourId, Colours::grey.brighter());
            sizeSlider->setColour(Slider::rotarySliderFillColourId, Colours::grey.brighter());
            
            audioProcessor.update();
        } else
        {
            audioProcessor.setButtonState(false);
            
            dampSlider->setColour(Slider::rotarySliderFillColourId, Colours::skyblue);
            sizeSlider->setColour(Slider::rotarySliderFillColourId, Colours::skyblue);
            
            
            audioProcessor.update();
        }
    }
}
