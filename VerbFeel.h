/*
  ==============================================================================

    LookAndFeel.h
    Created: 12 Jul 2020 8:49:21am
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class  VerbFeel : public juce::LookAndFeel_V4
{
public:
    VerbFeel();
    ~VerbFeel();
    
    void drawRotarySlider (Graphics&, int x, int y, int width, int height, float sliderPosProportional,
                           float rotaryStartAngle, float rotaryEndAngle, Slider&) override;
   
    void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    
    Font getTextButtonFont (TextButton& button, int buttonHeight) override;
    
//    void drawToggleButton(Graphics&, ToggleButton&,
//                          bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    
    juce::Slider::SliderLayout getSliderLayout(Slider&) override;
    Label* createSliderTextBox(Slider&) override;
    Font getLabelFont(Label&) override;
    
    
private:
    Font getFont()
    {
        return Font ("Consolas", "Regular", 11.f);
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VerbFeel)
};

