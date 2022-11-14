/*
  ==============================================================================

    Voice.h
    Created: 27 Sep 2022 4:28:59pm
    Author:  Luke

  ==============================================================================
*/

#pragma once

#include "Oscillator.h"
#include "Envelope.h"

struct Voice
{
    int note;
    float saw;
    
    Envelope env;
    Oscillator osc1;
    Oscillator osc2;
    
    void release()
    {
        env.release();
    }
    
    void reset()
    {
        note = 0;
        saw = 0.0f;
        env.reset();
        osc1.reset();
        osc2.reset();
    }
    
    float render(float input)
    {
        float sample1 = osc1.nextSample();
        float sample2 = osc2.nextSample();
        saw = (saw * 0.997f) + sample1 - sample2; // 'leaky integrator'
    
        float output = saw + input;
        float envelope = env.nextValue();
        
        return output * envelope;
    }
};
