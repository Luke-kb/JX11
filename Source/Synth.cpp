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
    voice.reset();
    noiseGen.reset();
}

void Synth::midiMessage(uint8_t data0, uint8_t data1, uint8_t data2)
{
    switch (data0  & 0xF0) {
        // note off
        case 0x80:
            noteOff(data1 & 0x7F);
            break;
        // note on
        case 0x90:
            uint8_t note = data1 & 0x7F;
            uint8_t velo = data2 & 0x7F;
            if (velo > 0) {
                noteOn(note, velo);
            } else {
                noteOff(note);
            }
            break;
    }
}

void Synth::noteOn(int note, int velocity)
{
    voice.note = note;
    
    float freq = 440.f * std::exp2(float(note - 69) / 12.f); // convert midi to freq
    voice.period = sampleRate / freq;
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
    if (voice.note == note) {
        voice.release();
    }
}

void Synth::render(float** outputBuffers, int sampleCount)
{
    float* outputBufferLeft = outputBuffers[0];
    float* outputBufferRight = outputBuffers[1];
    
    voice.osc1.period = voice.period;
    voice.osc2.period = voice.osc1.period * detune;

    
    // loop through samplesin buffer one-by-one
    for (int sample = 0; sample < sampleCount; ++sample) {
        // grab output from noise generator
        float noise = noiseGen.nextValue() * noiseMix;
        
        // set output to 0
        float output = 0.0f;
        
        // if a noteOn msg has been sent and a noteOff msg has NOT been received for the same key
        // calculate the new sample value by multiplying the noise by the velocity and then dividing
        // it by 127 (the total num of values) and multiplying by 0.5 (eg a 6dB reduction)
        // so that it is not too loud
        if (voice.env.isActive()) {
            output = voice.render(noise);
        }
        
        // write the output value to the respective output buffers
        // sound will appear mono as the same value is written to both
        // Left and Right buffers
        // if plugin is running in mono, OutputBufferRight will nullptr
        // and we only write to outputBufferLeft
        outputBufferLeft[sample] = output;
        if (outputBufferRight != nullptr) {
            outputBufferRight[sample] = output;
        }
    }
    
    if (!voice.env.isActive()) {
        voice.env.reset();
    }
    
    protectYourEars(outputBufferLeft, sampleCount);
    protectYourEars(outputBufferRight, sampleCount);
}

