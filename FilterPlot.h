/*
  ==============================================================================

    FilterPlot.h
    Created: 29 Oct 2020 1:32:46pm
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/
//
//#pragma once
//
//#include <JuceHeader.h>
//#include "PluginProcessor.h"
//
//
//class FilterPlot  : public juce::Component
//{
//public:
//    FilterPlot();
//    ~FilterPlot() override;
//
//    void paint (juce::Graphics&) override;
//    void resized() override;
//    /*==============================================================*/
//    class BandEditor : public Component,
//    public Button::Listener
//    {
//    public:
//        BandEditor (size_t i, TrailingDecayAudioProcessor& processor);
//        
//        void resized () override;
//        
//        void updateControls (TrailingDecayAudioProcessor::FilterType type);
//        
//        void updateSoloState (bool isSolo);
//        
//        void setFrequency (float frequency);
//        
//        void setGain (float gain);
//        
//        void setType (int type);
//        
//        void buttonClicked (Button* b) override;
//        
//        Path frequencyResponse;
//    private:
//        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BandEditor)
//        
//        size_t index;
//        TrailingDecayAudioProcessor& processor;
//        
//        GroupComponent      frame;
//        ComboBox            filterType;
//        Slider              frequency;
//        Slider              quality;
//        Slider              gain;
//        TextButton          solo;
//        TextButton          activate;
//        OwnedArray<AudioProcessorValueTreeState::ComboBoxAttachment> boxAttachments;
//        OwnedArray<AudioProcessorValueTreeState::SliderAttachment> attachments;
//        OwnedArray<AudioProcessorValueTreeState::ButtonAttachment> buttonAttachments;
//    };
//    /*==============================================================*/
//private:
//    void updateFrequencyResponses ();
//    
//    static float getPositionForFrequency (float freq);
//    
//    static float getFrequencyForPosition (float pos);
//    
//    static float getPositionForGain (float gain, float top, float bottom);
//    
//    static float getGainForPosition (float pos, float top, float bottom);
//    
//    Path                    frequencyResponse;
//    Path                    analyserPath;
//    
//    OwnedArray<BandEditor>  bandEditors;
//    Rectangle<int>          plotFrame;
//    
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterPlot)
//};

