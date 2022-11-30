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
    float volumeTrim;
    float vibrato, pwmDepth;
    int glideMode;
    float glideRate, glideBend;
    
    juce::LinearSmoothedValue<float> outputLevelSmoother;
    
    static constexpr int MAX_VOICES = 8;
    int numVoices;
    
    float velocitySensitivity;
    bool ignoreVelocity;
    
    const int LFO_MAX = 32; // how often the LFO is updated (in samples)
    float lfoIncrement;
    
private:
    void noteOn(int note, int velocity);
    void noteOff(int note);
    float calculatePeriod(int v, int note) const;
    int findFreeVoice() const;
    void controlChange(uint8_t data1, uint8_t data2);
    void restartMonoVoice(int note, int velocity);
    void shiftQueuedNotes();
    int nextQueuedNote();
    void updateLFO();
    
    
    int lfoStep;
    float lfo;
    float sampleRate;
    float pitchBend;
    bool sustainPedalPressed = false;
    float modWheel;
    int lastNote;
  
    inline void updatePeriod(Voice& voice)
    {
        voice.osc1.period = voice.period * pitchBend;
        voice.osc2.period = voice.osc1.period * detune;
    }
  
    bool isPlayingLegatoStyle() const;
    
    std::array<Voice, MAX_VOICES> voices;
    NoiseGenerator noiseGen;
};
