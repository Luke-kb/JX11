/*
  ==============================================================================

    Synth.cpp
    Created: 27 Sep 2022 4:28:38pm
    Author:  Luke

  ==============================================================================
*/

#include "Synth.h"
#include "Utils.h"

Synth::Synth()
{
    sampleRate = 44100.0f;
}

void Synth::allocateResources(double sampleRate_, int /*samplesPerBlock*/)
{
    sampleRate = static_cast<float>(sampleRate_);
}

void Synth::deallocateResources()
{
    // do nothing yet
}

void Synth::reset()
{
    for (int v = 0; v < MAX_VOICES; ++v) {
        voices[v].reset();
    }
    
    noiseGen.reset();
    pitchBend = 1.0f;
}

void Synth::midiMessage(uint8_t data0, uint8_t data1, uint8_t data2)
{
    switch (data0  & 0xF0) {
        // note off
        case 0x80:
            noteOff(data1 & 0x7F);
            break;
        // note on
        case 0x90: {
            uint8_t note = data1 & 0x7F;
            uint8_t velo = data2 & 0x7F;
            if (velo > 0) {
                noteOn(note, velo);
            } else {
                noteOff(note);
            }
            break;
        }
            
        // pitch bend
        case 0xE0:
            pitchBend = std::exp(-0.000014102f * float(data1 + 128 * data2 - 8192));
            break;
    }
}

void Synth::noteOn(int note, int velocity)
{
    int v = 0; // voice index (0 = mono voice)
    
    if (numVoices > 1) {
        v = findFreeVoice();
    }
    
    startVoice(v, note, velocity);
}

void Synth::startVoice(int v, int note, int velocity)
{
    float period = calculatePeriod(note);
    
    Voice& voice = voices[v];
    voice.period = period;
    voice.note = note;
    voice.updatePanning();
    voice.osc1.amplitude = (velocity / 127.0f) * 0.5f;
    voice.osc2.amplitude = voice.osc1.amplitude * oscMix;

    Envelope& env = voice.env;
    env.attackMultiplier = envAttack;
    env.decayMultiplier = envDecay;
    env.sustainLevel = envSustain;
    env.releaseMultiplier = envRelease;
    env.attack();
}

void Synth::noteOff(int note)
{
    for (int v = 0; v < MAX_VOICES; v++) {
        if (voices[v].note == note) {
            voices[v].release();
            voices[v].note = 0;
        }
    }
}

int Synth::findFreeVoice() const
{
    int v = 0;
    float l = 100.f;
    
    for (int i = 0; i < MAX_VOICES; ++i) {
        if (voices[i].env.level < l && !voices[i].env.isInAttack()) {
            l = voices[i].env.level;
            v = i;
        }
    }
    return v;
}

float Synth::calculatePeriod(int note) const
{
    float period = tune * std::exp(-0.05776226505f * float(note));
    while (period < 6.0f || (period * detune) < 6.0f) { period += period; }
    return period;
}

void Synth::render(float** outputBuffers, int sampleCount)
{
    float* outputBufferLeft = outputBuffers[0];
    float* outputBufferRight = outputBuffers[1];
    
    // apply pitchbend & detune for currently rendered voices
    for (int v = 0; v < MAX_VOICES; ++v) {
        Voice& voice = voices[v];
        if (voice.env.isActive()) {
            voice.osc1.period = voice.period * pitchBend;
            voice.osc2.period = voice.osc1.period * detune;
        }
    }
    
    // loop through samplesin buffer one-by-one
    for (int sample = 0; sample < sampleCount; ++sample) {
        // grab output from noise generator
        float noise = noiseGen.nextValue() * noiseMix;
        
        // set outputs to 0
        float outputLeft = 0.0f;
        float outputRight = 0.0f;
        
        // render any active voices
        for (int v = 0; v < MAX_VOICES; ++v) {
            Voice& voice = voices[v];
            if (voice.env.isActive()) {
                float output = voice.render(noise);
                outputLeft += output * voice.panLeft;
                outputRight += output * voice.panRight;
            }
        }
        
        // write the output value to the respective output buffers
        // if plugin is running in mono, OutputBufferRight will nullptr
        // and we only write to outputBufferLeft
        if (outputBufferRight != nullptr) {
            outputBufferLeft[sample] = outputLeft;
            outputBufferRight[sample] = outputRight;
        } else {
            outputBufferLeft[sample] = (outputLeft + outputRight) * 0.5f;
        }
    }
    // resets voices that are not active
    for (int v = 0; v < MAX_VOICES; ++v) {
        Voice& voice = voices[v];
        if (!voice.env.isActive()) {
            voice.env.reset();
        }
    }
        
    
    protectYourEars(outputBufferLeft, sampleCount);
    protectYourEars(outputBufferRight, sampleCount);
}

