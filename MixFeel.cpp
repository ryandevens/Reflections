
/*
 ==============================================================================
 
 LookAndFeel.cpp
 Created: 12 Jul 2020 8:49:21am
 Author:  Billie (Govnah) Jean
 
 ==============================================================================
 */

#include "MixFeel.h"

MixFeel::MixFeel()
{
    setColour(juce::Slider::backgroundColourId, Colours::grey);
    setColour(juce::Slider::trackColourId, Colours::hotpink.withLightness(0.8f));
    
}

MixFeel::~MixFeel()
{
    
}



/*=================================================================================*/
/*----------------------------- drawRotarySlider ----------------------------------*/
/*=================================================================================*/
void MixFeel::drawLinearSliderBackground (Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle, Slider& slider)
{
    
    juce::Rectangle<int> bgRect;
    bgRect.setX(x);
    bgRect.setY(y);
    bgRect.setSize(width, height);
    g.setColour(juce::Colours::grey);
    g.drawRoundedRectangle(bgRect.toFloat(), 1.f, 2.f);
}

juce::Slider::SliderLayout MixFeel::getSliderLayout(Slider& slider)
{
    Slider::SliderLayout layout;
    auto bounds = slider.getLocalBounds();
    
    juce::Rectangle<int>textBox = bounds.removeFromBottom (15);
    textBox.setCentre(35, 53);
    layout.textBoxBounds = textBox;
    layout.sliderBounds  = bounds;
    
    
//   layout.sliderBounds = lRecangle<int> (0, 0, 75, 75x);    
//   layout.textBoxBounds = lRecangle<int> (0, 50, 75, 10);
    
    return layout;
}


Label* MixFeel::createSliderTextBox(Slider& slider)
{
   Label* l = LookAndFeel_V4::createSliderTextBox(slider);
    // make sure editor text is black (so it shows on white background)
    l->setColour(juce::Label::outlineColourId, juce::Colours::white.withAlpha(0.0f));
    l->setColour(juce::Slider::textBoxBackgroundColourId, Colours::hotpink);
    l->setColour(juce::Label::textColourId, Colours::hotpink);
    
    
    return l;
}

/*=================================== FONT =========================================*/

Font MixFeel::getLabelFont(Label& label)
{
   return getFont();
    
}







