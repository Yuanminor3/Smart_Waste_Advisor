// audio_logic.h
#ifndef AUDIO_LOGIC_H
#define AUDIO_LOGIC_H

#include <Arduino.h>

// Initialize audio system (I2S + speech engine)
void audio_init();

// Speak out a label using text-to-speech
void audio_speak(const char* label);

#endif  // AUDIO_LOGIC_H
