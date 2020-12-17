/*
  ==============================================================================

    LookAndFeel.cpp
    Created: 12 Jul 2020 8:49:21am
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#include "DelayFeel.h"

DelayFeel::DelayFeel()
{
    
    setColour(juce::Slider::rotarySliderFillColourId, Colours::orange);
    setColour(juce::TextButton::textColourOnId, Colours::orange);
    setColour(juce::TextButton::textColourOffId, Colours::orange.darker());
    setColour(juce::TextButton::buttonColourId, Colours::black);
    
    
    
    
    //setColour(juce::Label::backgroundColourId, Colours::white);
}

DelayFeel::~DelayFeel()
{
    
}



/*=================================================================================*/
/*----------------------------- drawRotarySlider ----------------------------------*/
/*=================================================================================*/
void DelayFeel::drawRotarySlider (Graphics& g, int x, int y, int width, int height,
                                     float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider& slider)
{
    auto radius = jmin (width / 2, height / 2) - 2.0f;
    auto centreX = x + width  * 0.5f;
    auto centreY = y + height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();
    
    
    g.setColour(slider.findColour(Slider::textBoxOutlineColourId));
    
    if (slider.isEnabled())
        g.setColour (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (isMouseOver ? 1.0f : 0.7f));
    else
        g.setColour (Colour (0x80808080));
    
    {
        Path filledArc;
        filledArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, angle, 0.0);
        g.fillPath (filledArc);
    }
    
    {
        auto lineThickness = jmin (15.0f, jmin (width, height) * 0.45f) * 0.1f;
        Path outlineArc;
        outlineArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, 0.0);
        g.strokePath (outlineArc, PathStrokeType (lineThickness));
    }
    
   
}

void DelayFeel::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour,
                                     bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    auto area = button.getLocalBounds().toFloat();
    
    if(button.getToggleState())
    {
        g.setColour(button.findColour(TextButton::buttonColourId));
        g.setColour(Colours::orange.brighter());
        g.drawRoundedRectangle(area, 2.f, 1.f);
        g.setColour(Colours::orange.brighter(0.9f).withAlpha(0.2f));
    } else
    {
        g.setColour(button.findColour(TextButton::buttonOnColourId));
        g.setColour(Colours::orange.darker(0.8f));
        g.drawRoundedRectangle(area, 2.f, 1.f);
        g.setColour(Colours::orange.withAlpha(0.2f));
        
    }
    
    g.fillAll();
}

Font DelayFeel::getTextButtonFont (TextButton& button, int buttonHeight)
{
    return getFont();
}



juce::Slider::SliderLayout DelayFeel::getSliderLayout(Slider& slider)
{
    Slider::SliderLayout layout;
    auto bounds = slider.getLocalBounds();
    
    Rectangle<int> textBox = bounds.removeFromBottom (15);
    textBox.setCentre(35, 53);
    layout.textBoxBounds = textBox;
    layout.sliderBounds  = bounds;
    
//    layout.sliderBounds = Rectangle<int> (0, 0, 75, 75);
//    layout.textBoxBounds = Rectangle<int> (0, 50, 75, 10);

    return layout;
}

Label* DelayFeel::createSliderTextBox(Slider& slider)
{
    Label* l = LookAndFeel_V4::createSliderTextBox(slider);
    
    
    
    // make sure editor text is black (so it shows on white background)
    //l->setColour(juce::Label::outlineColourId, Colours::white.withAlpha(0.0f));
    l->setColour(juce::Slider::textBoxBackgroundColourId, Colours::orange);
    l->setColour(juce::Label::textColourId, Colours::orange);
    
    return l;
}

/*=================================== FONT =========================================*/

Font DelayFeel::getLabelFont(Label& label)
{
    return getFont();
}







