/*
  ==============================================================================

    Oscillator.h
    Created: 2 Oct 2022 8:03:14am
    Author:  Luke

  ==============================================================================
*/

#pragma once

const float TWO_PI = 6.2831853071795864f;

class Oscillator
{
public:
    float amplitude;
    float inc;  // increment
    float phase;
    
    void reset()
    {
        phase = 0.0f;
        
        // sine wave
//        sin0 = amplitude * std::sin(phase * TWO_PI);
//        sin1 = amplitude * std::sin((phase - inc) * TWO_PI);
//        dsin = 2.f * std::cos(inc * TWO_PI);
    }
    
    float nextSample()
    {
        // sine wave
//        float sinx = dsin * sin0 - sin1;
//        sin1 = sin0;
//        sin0 = sinx;
//
//        return sinx;
        
        // sawtooth wave
        phase += inc;
        if (phase >= 1.0f) {
            phase -= 1.0f;
        }
        return amplitude * (2.0f * phase - 1.0f);
    }

private:
    float sin0;
    float sin1;
    float dsin;
};
