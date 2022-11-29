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
    float period;
    float panLeft, panRight;
    
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
        panLeft = 0.707f;
        panRight = 0.707f;
    }
    
    void updatePanning()
    {
        float panning = std::clamp((note - 60.0f) / 24.0f, -1.0f, 1.0f); // converts midi to panning value
        panLeft = std::sin(PI_OVER_4 * (1.0f - panning));
        panRight = std::sin(PI_OVER_4 * (1.0f + panning));
    }
    
    float render(float input)
    {
        float sample1 = osc1.nextSample();
        float sample2 = osc2.nextSample();
        saw = (saw * 0.997f) + sample1 - sample2; // 'leaky integrator'
        // saw = sample1 - sample2; // calculate saw this way to view only the sinc pulses
        float output = saw + input;
        float envelope = env.nextValue();
        
        return output * envelope;
    }
};
