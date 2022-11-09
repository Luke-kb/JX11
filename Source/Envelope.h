/*
  ==============================================================================

    Envelope.h
    Created: 8 Nov 2022 9:20:25pm
    Author:  Luke

  ==============================================================================
*/

#pragma once

const float SILENCE = 0.0001f;

class Envelope
{
public:
    
    float multiplier;
    float level;
    
    float nextValue()
    {
        level *= multiplier;
        return level;
    }
};
