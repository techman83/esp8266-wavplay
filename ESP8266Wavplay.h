

#ifndef __ESP8266WAVPLAY_H__

#include "Arduino.h"
#include "i2s.h"
#include "i2s_reg.h"
#include "ESP8266Wavspiffs.h"


#ifdef __cplusplus
extern "C" {
#endif

void wavSetup();
void wavStartPlaying(const char *wavfilename);
bool wavPlaying();
void wavStopPlaying();
void wavLoop();
void showDir();
int16_t Amplify(int16_t s);
bool setGain(float f);

#ifdef __cplusplus
}
#endif

#endif
#define __ESP8266WAVPLAY_H__   1
