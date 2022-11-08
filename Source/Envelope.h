/*
  ==============================================================================

    Envelope.h
    Created: 8 Nov 2022 9:20:25pm
    Author:  Luke

  ==============================================================================
*/

#pragma once

class Envelope
{
public:
    float nextValue()
    {
        level *= 0.9999f;
        return level;
    }
    
    float level;
};
