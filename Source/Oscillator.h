/*
  ==============================================================================

    Oscillator.h
    Created: 2 Oct 2022 8:03:14am
    Author:  Luke

  ==============================================================================
*/

#pragma once

#include <cmath>

const float PI_OVER_4 = 0.7853981633974483f;
const float PI = 3.1415926535897932f;
const float TWO_PI = 6.2831853071795864f;

class Oscillator
{
public:
    float period = 0.0f;
    float amplitude = 1.0f;
    
    void reset()
    {
        inc = 0.0f;
        phase = 0.0f;
    }
    
    float nextSample()
    {
        float output = 0.0f;
        
        phase += inc;
        
        if (phase <= PI_OVER_4) {
            
            float halfPeriod = period / 2.0f; // midpoint between current peak and the next one
            
            phaseMax = std::floor(0.5f + halfPeriod) - 0.5f;
            phaseMax *= PI;
            
            inc = phaseMax / halfPeriod;
            phase = -phase;
            
            if ((phase * phase) > 1e-9) { // aka: if (phase < -1e-9 || phase > 1e-9)
                output = (amplitude * std::sin(phase)) / phase;
            } else {
                output = amplitude;
            }
        } else {
            if (phase > phaseMax) {
                phase = (phaseMax + phaseMax) - phase;
                inc = -inc;
            }
            
            output = (amplitude * std::sin(phase)) / phase;
        }
        
        return output;
    }
    
private:
    float phase; // measured as: sample * PI
    float phaseMax; // measured as: sample * PI
    float inc;
};
