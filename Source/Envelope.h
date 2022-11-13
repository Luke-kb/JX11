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
    inline bool isActive() const
    {
        return level > SILENCE;
    }
    
    inline bool isInAttack() const
    {
        return target >= 2.0f;
    }
    
    void attack()
    {
        level += SILENCE + SILENCE; // boosts the env level so isActive() returns true
        target = 2.0f;
        multiplier = attackMultiplier;
    }
    
    void release()
    {
        target = 0.0f;
        multiplier = releaseMultiplier;
    }
    
    float nextValue()
    {
        level = multiplier * (level - target) + target;
        
        // switch to decay stage
        if (level + target > 3.0f) {
            multiplier = decayMultiplier;
            target = sustainLevel;
        }
        
        return level;
    }
    
    void reset()
    {
        level = 0.0f;
        target = 0.0f;
        multiplier = 0.0f;
    }
    
    float level;
    float sustainLevel;
    float attackMultiplier, decayMultiplier, releaseMultiplier;
    
private:
    float multiplier;
    float target;
};
