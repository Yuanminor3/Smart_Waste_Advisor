// audio_logic.cpp
#include "audio_logic.h"
#include <AudioOutputI2S.h>
#include <ESP8266SAM.h>

// I2S pin definitions
#define I2S_BCLK 1    // Bit Clock - GPIO1
#define I2S_LRCLK 3   // Left/Right Clock - GPIO3
#define I2S_DOUT 5    // Data Out - GPIO5

// Audio output and speech synthesizer objects
static AudioOutputI2S* out = nullptr;
static ESP8266SAM* sam = nullptr;

// Set up audio output and TTS engine
void audio_init() {
  out = new AudioOutputI2S();
  out->SetPinout(I2S_BCLK, I2S_LRCLK, I2S_DOUT);
  out->SetGain(0.4);  // Volume (0.0 to 1.0)

  sam = new ESP8266SAM();
  sam->SetVoice(ESP8266SAM::VOICE_SAM);  // Use classic SAM voice
  sam->SetSpeed(90);     // Speaking speed (72–135 normal range)
  sam->SetPitch(90);     // Voice pitch
  sam->SetThroat(128);   // Timbre - throat
  sam->SetMouth(128);    // Timbre - mouth
}

// Speak the given text label
void audio_speak(const char* label) {
  if (sam && out && label && strlen(label) > 0) {
    sam->Say(out, label);
  }
}
