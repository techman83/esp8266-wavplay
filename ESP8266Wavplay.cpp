/*****************************************************************************
  The MIT License (MIT)
  Copyright (c) 2016 by bbx10node@gmail.com
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
 **************************************************************************/

#include "ESP8266Wavplay.h"

bool ICACHE_FLASH_ATTR i2s_write_lr_nb(int16_t left, int16_t right){
  int sample = right & 0xFFFF;
  sample = sample << 16;
  sample |= left & 0xFFFF;
  return i2s_write_sample_nb(sample);
}

struct I2S_status_s {
  wavFILE_t wf;
  int16_t buffer[512];
  int bufferlen;
  int buffer_index;
  int playing;
} I2S_WAV;

void wavStopPlaying()
{
  i2s_end();
  I2S_WAV.playing = false;
  wavClose(&I2S_WAV.wf);
}

bool wavPlaying()
{
  return I2S_WAV.playing;
}

void wavSetup()
{
  Serial.println(F("wavSetup"));
  bool result = SPIFFS.begin();
  Serial.println("SPIFFS opened: " + result);
  I2S_WAV.bufferlen = -1;
  I2S_WAV.buffer_index = 0;
  I2S_WAV.playing = false;
}

void wavLoop()
{
  bool i2s_full = false;
  int rc;

  while (I2S_WAV.playing && !i2s_full) {
    while (I2S_WAV.buffer_index < I2S_WAV.bufferlen) {
      int16_t pcm = I2S_WAV.buffer[I2S_WAV.buffer_index];
      if (i2s_write_lr_nb(pcm, pcm)) {
        I2S_WAV.buffer_index++;
      }
      else {
        i2s_full = true;
        break;
      }
      if ((I2S_WAV.buffer_index & 0x3F) == 0) yield();
    }
    if (i2s_full) break;

    rc = wavRead(&I2S_WAV.wf, I2S_WAV.buffer, sizeof(I2S_WAV.buffer));
    if (rc > 0) {
      //Serial.printf("wavRead %d\r\n", rc);
      I2S_WAV.bufferlen = rc / sizeof(I2S_WAV.buffer[0]);
      I2S_WAV.buffer_index = 0;
    }
    else {
      Serial.println(F("Stop playing"));
      wavStopPlaying();
      break;
    }
  }
}

void wavStartPlaying(const char *wavfilename)
{
  wavProperties_t wProps;
  int rc;

  Serial.printf("wavStarPlayingFile(%s)\r\n", wavfilename);
  i2s_begin();
  rc = wavOpen(wavfilename, &I2S_WAV.wf, &wProps);
  Serial.printf("wavOpen %d\r\n", rc);
  if (rc != 0) {
    Serial.println("wavOpen failed");
    return;
  }
  Serial.printf("audioFormat %d\r\n", wProps.audioFormat);
  Serial.printf("numChannels %d\r\n", wProps.numChannels);
  Serial.printf("sampleRate %d\r\n", wProps.sampleRate);
  Serial.printf("byteRate %d\r\n", wProps.byteRate);
  Serial.printf("blockAlign %d\r\n", wProps.blockAlign);
  Serial.printf("bitsPerSample %d\r\n", wProps.bitsPerSample);

  i2s_set_rate(wProps.sampleRate);

  I2S_WAV.bufferlen = -1;
  I2S_WAV.buffer_index = 0;
  I2S_WAV.playing = true;
  wavLoop();
}

void showDir(void)
{
  wavFILE_t wFile;
  wavProperties_t wProps;
  int rc;

  Serial.println("Checking files");
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    Serial.println(dir.fileName());
    rc = wavOpen(dir.fileName().c_str(), &wFile, &wProps);
    if (rc == 0) {
      Serial.printf("  audioFormat %d\r\n", wProps.audioFormat);
      Serial.printf("  numChannels %d\r\n", wProps.numChannels);
      Serial.printf("  sampleRate %d\r\n", wProps.sampleRate);
      Serial.printf("  byteRate %d\r\n", wProps.byteRate);
      Serial.printf("  blockAlign %d\r\n", wProps.blockAlign);
      Serial.printf("  bitsPerSample %d\r\n", wProps.bitsPerSample);
      Serial.println();
      wavClose(&wFile);
    }
  }
}
