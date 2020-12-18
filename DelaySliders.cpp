/*
  ==============================================================================

    DelaySliders.cpp
    Created: 10 Oct 2020 6:07:47pm
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DelaySliders.h"


//==============================================================================
DelaySliders::DelaySliders(ReflectionsAudioProcessor& processor) : mProcessor(processor)
{
    setLookAndFeel(&delayFeel);
    /*------------------------------------------------------------------------*/
    button = std::make_unique<TextButton>("Link");
    button->setBounds(121, 95, 30, 15);
    button->setClickingTogglesState(true);
    button->addListener(this);
    addAndMakeVisible(button.get());
    
    syncButton = std::make_unique<TextButton>("Sync");
    syncButton->setBounds(121, 71, 30, 15);
    syncButton->setClickingTogglesState(true);
    syncButton->addListener(this);
    addAndMakeVisible(syncButton.get());


    /*------------------------------------------------------------------------*/
    delaySliderL = std::make_unique<Slider>(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextBoxBelow);
    delaySliderL->setBounds(33, 25, 70, 70);
    addAndMakeVisible (delaySliderL.get());
    delaySliderL->addListener(this);
    
    leftLabel = std::make_unique<Label>("", "Delay Left");
    addAndMakeVisible (leftLabel.get());
    leftLabel->attachToComponent (delaySliderL.get(), false);
    leftLabel->setJustificationType (Justification::centred);
    
    syncLabelL = std::make_unique<Label>("", "1/4");
    syncLabelL->setBounds(0, 0, 10, 10);
    addAndMakeVisible (syncLabelL.get());
    //syncLabelL->attachToComponent (delaySliderL.get(), true);
    //syncLabelL->setJustificationType (Justification::centred);
    
    /*------------------------------------------------------------------------*/
    delaySliderR = std::make_unique<Slider>(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextBoxBelow);
    delaySliderR->setBounds(172, 25, 70, 70);
    addAndMakeVisible (delaySliderR.get());
    delaySliderR->addListener(this);
    
    rightLabel = std::make_unique<Label>("", "Delay Right");
    addAndMakeVisible (rightLabel.get());
    rightLabel->attachToComponent (delaySliderR.get(), false);
    rightLabel->setJustificationType (Justification::centred);
    
    /*------------------------------------------------------------------------*/
    feedbackSliderL = std::make_unique<Slider>(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextBoxBelow);
    feedbackSliderL->setBounds(33, 105, 70, 70);
    addAndMakeVisible (feedbackSliderL.get());
    feedbackSliderL->addListener(this);
    
    fbLabelL = std::make_unique<Label>("", "Feedback Left");
    addAndMakeVisible (fbLabelL.get());
    fbLabelL->attachToComponent (feedbackSliderL.get(), false);
    fbLabelL->setJustificationType (Justification::centred);
    
    /*------------------------------------------------------------------------*/
    feedbackSliderR = std::make_unique<Slider>(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextBoxBelow);
    feedbackSliderR->setBounds(172, 105, 70, 70);
    addAndMakeVisible (feedbackSliderR.get());
    feedbackSliderR->addListener(this);
    
    fbLabelR = std::make_unique<Label>("", "Feedback Right");
    addAndMakeVisible (fbLabelR.get());
    fbLabelR->attachToComponent (feedbackSliderR.get(), false);
    fbLabelR->setJustificationType (Justification::centred);
    
    /*------------------------------------------------------------------------*/
    delayInputSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextBoxBelow);
    delayInputSlider->setBounds(25, 200, 70, 70);
    addAndMakeVisible (delayInputSlider.get());

    delayInputLabel = std::make_unique<Label>("", "Dry Input");
    addAndMakeVisible (delayInputLabel.get());
    delayInputLabel->attachToComponent (delayInputSlider.get(), false);
    delayInputLabel->setJustificationType (Justification::centred);

    /*------------------------------------------------------------------------*/
    delayOutputSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextBoxBelow);
    delayOutputSlider->setBounds(100, 200, 70, 70);
    addAndMakeVisible (delayOutputSlider.get());

    delayOutputLabel = std::make_unique<Label>("", "Delay Output");
    addAndMakeVisible (delayOutputLabel.get());
    delayOutputLabel->attachToComponent (delayOutputSlider.get(), false);
    delayOutputLabel->setJustificationType (Justification::centred);

    /*------------------------------------------------------------------------*/
    delaySendSlider = std::make_unique<Slider>(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextBoxBelow);
    delaySendSlider->setBounds(175, 200, 70, 70);
    addAndMakeVisible (delaySendSlider.get());

    delaySendLabel = std::make_unique<Label>("", "Delay Send");
    addAndMakeVisible (delaySendLabel.get());
    delaySendLabel->attachToComponent (delaySendSlider.get(), false);
    delaySendLabel->setJustificationType (Justification::centred);
    
    /*------------------------------------------------------------------------*/
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    delayAttachmentL = std::make_unique<Attachment>(mProcessor.apvts, "Time L", *delaySliderL);
    delayAttachmentR = std::make_unique<Attachment>(mProcessor.apvts, "Time R", *delaySliderR);
    feedbackAttachmentL = std::make_unique<Attachment>(mProcessor.apvts, "FB L", *feedbackSliderL);
    feedbackAttachmentR = std::make_unique<Attachment>(mProcessor.apvts, "FB R", *feedbackSliderR);
    
    delayInputAttachment = std::make_unique<Attachment>(mProcessor.apvts, "Delay Input", *delayInputSlider);
    delayOutputAttachment = std::make_unique<Attachment>(mProcessor.apvts, "Delay Output", *delayOutputSlider);
    delaySendAttachment = std::make_unique<Attachment>(mProcessor.apvts, "Delay Send", *delaySendSlider);
    
    
    buttonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(mProcessor.apvts, "Delay Link", *button);
    syncButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(mProcessor.apvts, "Sync", *syncButton);
    
    auto& lnf = getLookAndFeel();
    lnf.setColour (Slider::textBoxTextColourId, Colours::brown.darker());
    lnf.setColour (Slider::textBoxHighlightColourId, Colours::whitesmoke);
    lnf.setColour (Slider::textBoxOutlineColourId, Colours::transparentWhite);
    
    sendLookAndFeelChange();
    
    
    setSize (275, 275);

}
/*==========================================================*/
DelaySliders::~DelaySliders()
{
}

/*==========================================================*/
void DelaySliders::paint (juce::Graphics& g)
{
    g.setColour(Colours::black);
    g.fillAll();
    
    g.setColour (juce::Colours::orange);
    
    juce::Path vertPath;
    vertPath.startNewSubPath (juce::Point<float> (134, 10));
    vertPath.lineTo (juce::Point<float> (135, 10));
    vertPath.lineTo (juce::Point<float> (135, 69));
    vertPath.lineTo (juce::Point<float> (134, 69));
    vertPath.closeSubPath();
    
    juce::Path vertPath2;
    vertPath2.startNewSubPath (juce::Point<float> (134, 112));
    vertPath2.lineTo (juce::Point<float> (135, 112));
    vertPath2.lineTo (juce::Point<float> (135, 167));
    vertPath2.lineTo (juce::Point<float> (134, 167));
    vertPath2.closeSubPath();
    
    
    if (paintConnected)
    {
        juce::Path delayConnection;
        delayConnection.startNewSubPath(juce::Point<float> (100, 43));
        delayConnection.lineTo(juce::Point<float> (100, 44));
        delayConnection.lineTo(juce::Point<float> (172, 44));
        delayConnection.lineTo(juce::Point<float> (172, 43));
        delayConnection.closeSubPath();
        g.fillPath(delayConnection);
        
        juce::Path delayConnection2;
        delayConnection2.startNewSubPath(juce::Point<float> (100, 130));
        delayConnection2.lineTo(juce::Point<float> (100, 131));
        delayConnection2.lineTo(juce::Point<float> (172, 131));
        delayConnection2.lineTo(juce::Point<float> (172, 130));
        delayConnection2.closeSubPath();
        g.fillPath(delayConnection2);
    }
    
    g.fillPath(vertPath);
    g.fillPath(vertPath2);
    
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

/*==========================================================*/
void DelaySliders::resized()
{

}

/*==========================================================*/
void DelaySliders::sliderValueChanged(Slider* slider)
{
    repaint();
    if (mProcessor.getDelayButtonState())
    {
        if (slider->getMaximum() > 100.0)
        {
            delaySliderR->setValue(slider->getValue());
            delaySliderL->setValue(slider->getValue());
        } else
        {
            feedbackSliderR->setValue(slider->getValue());
            feedbackSliderL->setValue(slider->getValue());
        }
    }
}

/*==========================================================*/
void DelaySliders::buttonClicked(Button* mButton)
{
    if (mButton == button.get())
    {
        if (mProcessor.getDelayButtonState() == false)
        {
            mProcessor.setDelayButtonState(true);
            paintConnected = true;
            repaint();
            delaySliderR->setValue(delaySliderL->getValue());
            feedbackSliderR->setValue(feedbackSliderL->getValue());
            
            mProcessor.update();
        } else
        {
            mProcessor.setDelayButtonState(false);
            paintConnected = false;
            repaint();
            mProcessor.update();
        }
    }
    
    /*------------------------------------------------------------------------*/
    if (mButton == syncButton.get())
    {
        if (mProcessor.getSyncButtonState() == false)
        {
            delaySliderL->textFromValueFunction = [](float sliderValue)
            {
                if (sliderValue < 400)
                    return "1/16";
                if (sliderValue > 400 && sliderValue <= 800)
                    return "1/8";
                if (sliderValue > 800 && sliderValue <= 1200)
                    return "1/4";
                if (sliderValue > 1200 && sliderValue <= 1600)
                    return "1/2";
                if (sliderValue > 1600 && sliderValue <= 2000)
                    return "1/1";
                // How I would like to do this part, but lambda won't allow it (i think!)
//                juce::String syncString = tempoSync.getSyncString(value);
//                return syncString;
            };

            delaySliderR->textFromValueFunction = [](float sliderValue)
            {
                if (sliderValue <= 400)
                    return "1/16";
                if (sliderValue > 400 && sliderValue <= 800)
                    return "1/8";
                if (sliderValue > 800 && sliderValue <= 1200)
                    return "1/4";
                if (sliderValue > 1200 && sliderValue <= 1600)
                    return "1/2";
                if (sliderValue > 1600 && sliderValue <= 2000)
                    return "1/1";
            };
            //This part is to trigger the value change IE make the display change
            delaySliderL->setValue(delaySliderL->getValue()+1, sendNotification);
            delaySliderR->setValue(delaySliderR->getValue()+1, sendNotification);
            repaint();
            mProcessor.update();
        }
        else
        {
            delaySliderL->textFromValueFunction = [](float sliderValue)
            {
                return juce::String(sliderValue);
            };
            delaySliderR->textFromValueFunction = [](float sliderValue)
            {
                return juce::String(sliderValue);
            };
            delaySliderL->setValue(delaySliderL->getValue()+1, sendNotification);
            delaySliderR->setValue(delaySliderR->getValue()+1, sendNotification);
            repaint();
            mProcessor.update();
        }
    
    }
}
