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
    void startVoice(int v, int note, int velocity);
        
    float envAttack, envDecay, envSustain, envRelease;
    float noiseMix, oscMix, detune, tune;
    
    static constexpr int MAX_VOICES = 8;
    int numVoices;
    
private:
    void noteOn(int note, int velocity);
    void noteOff(int note);
    float calculatePeriod(int v, int note) const;
    int findFreeVoice() const;
    void controlChange(uint8_t data1, uint8_t data2);
    
    std::array<Voice, MAX_VOICES> voices;
    NoiseGenerator noiseGen;
    
    float sampleRate;
    float pitchBend;
    bool sustainPedalPressed = false;
};
