/*
  ==============================================================================

    Synth.h
    Created: 27 Sep 2022 4:28:38pm
    Author:  Luke

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Voice.h"
#include "NoiseGenerator.h"

class Synth
{
public:
    Synth();
    
    void allocateResources(double sampleRate, int samplesPerBlock);
    void deallocateResources();
    void midiMessage(uint8_t data0, uint8_t data1, uint8_t data2);
    void render(float** outputBuffers, int sampleCount);
    void reset();
        
    float envAttack, envDecay, envSustain, envRelease;
    float noiseMix, oscMix, detune, tune;
    
private:
    Voice voice;
    NoiseGenerator noiseGen;
    float sampleRate;

    void noteOn(int note, int velocity);
    void noteOff(int note);
    float calculatePeriod(int note) const;
};
