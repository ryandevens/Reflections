/*
  ==============================================================================

    LookAndFeel.cpp
    Created: 12 Jul 2020 8:49:21am
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#include "VerbFeel.h"

VerbFeel::VerbFeel()
{
    setColour(juce::Slider::rotarySliderFillColourId, Colours::powderblue.withLightness(0.6f));
    setColour(juce::TextButton::textColourOnId, Colours::white);
    setColour(juce::TextButton::textColourOffId, Colours::white);
    setColour(juce::TextButton::buttonColourId, Colours::powderblue.withLightness(0.6f).darker());
    setColour(juce::TextButton::buttonOnColourId, Colours::powderblue.withLightness(0.6f).darker(0.6f));
}

VerbFeel::~VerbFeel()
{
    
}



/*=================================================================================*/
/*----------------------------- drawRotarySlider ----------------------------------*/
/*=================================================================================*/
void VerbFeel::drawRotarySlider (Graphics& g, int x, int y, int width, int height,
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
        g.setColour((slider.findColour(Slider::rotarySliderFillColourId)).darker(0.8f));
        g.strokePath (outlineArc, PathStrokeType (lineThickness));
    }
    
   
}

juce::Slider::SliderLayout VerbFeel::getSliderLayout(Slider& slider)
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

void VerbFeel::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour,
                                     bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    if(button.getToggleState())
    {
        g.setColour(button.findColour(TextButton::buttonOnColourId));
        g.fillAll();
    } else
    {
        g.setColour(button.findColour(TextButton::buttonColourId));
        g.fillAll();
        g.setColour(button.findColour(TextButton::buttonOnColourId));
        g.drawRoundedRectangle(button.getLocalBounds().toFloat(), 2.f, 2.f);
    }
    
    
}

Font VerbFeel::getTextButtonFont (TextButton& button, int buttonHeight)
{
    //return Font (20.f);
    return getFont();
}


Label* VerbFeel::createSliderTextBox(Slider& slider)
{
    Label* l = LookAndFeel_V4::createSliderTextBox(slider);
    
    //l->setColour(juce::Slider::textBoxBackgroundColourId, Colours::powderblue.withLightness(0.6f));
    l->setColour(juce::Label::textColourId, Colours::powderblue.withLightness(0.6f));
    
    
    return l;
}

/*=================================== FONT =========================================*/

Font VerbFeel::getLabelFont(Label& label)
{
    return getFont();
}





