#ifndef __ESP8266WAVPLAY_H__

#include "Arduino.h"
#include "i2s.h"
#include "i2s_reg.h"
#include "wavspiffs.h"

#ifdef __cplusplus
extern "C" {
#endif

void wavSetup();
void wavStartPlaying(const char *wavfilename);
bool wavPlaying();
void wavStopPlaying();
void wavLoop();
void showDir();

#ifdef __cplusplus
}
#endif

#endif
#define __ESP8266WAVPLAY_H__   1
