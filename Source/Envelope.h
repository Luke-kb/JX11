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
    float target;
    float attackMultiplier;
    float decayMultiplier;
    float sustainLevel;
    float releaseMultiplier;
    
    float nextValue()
    {
        level = multiplier * (level - target) + target;
        return level;
    }
    
    void reset()
    {
        level = 0.0f;
        target = 0.0f;
        multiplier = 0.0f;
    }
};
