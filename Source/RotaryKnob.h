/*
 ==============================================================================
 
 RotaryKnob.h
 Created: 3 Dec 2022 5:42:55pm
 Author:  Luke
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
 */
class RotaryKnob  : public juce::Component
{
public:
  RotaryKnob();
  ~RotaryKnob() override;
  
  void paint (juce::Graphics&) override;
  void resized() override;
  
  juce::Slider slider;
  juce::String label;
  
private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotaryKnob)
};
