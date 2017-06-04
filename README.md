ESP8266 WavPlay
---------------

This is the core non blocking wav playing component of bbx10's [SFX I2S Web Trigger](https://github.com/bbx10/SFX-I2S-web-trigger) repackaged as a re-usable library. Plays wav files stored on the SPIFFS file system.

Should work on any ESP8266 based Arduino + I2S DAC, I've however only tested on a Wemos D1 Mini paired up with a [Adafruit MAX98357 I2S Class-D mono amp](https://learn.adafruit.com/adafruit-max98357-i2s-class-d-mono-amp/)

Basic Example
```c++
#include "wavplay.h"

void setup() {
  wavSetup();
}

void loop() {
  if (! wavPlaying()) {
    wavStartPlaying("/file.wav");
  }
  wavLoop();
}
```

Methods
-------
* wavSetup()

  Sets up the SPIFFS filesystem + initialises i2s

* wavStartPlaying("/filename.wav")
  
  Plays the filename from SPIFFs

* wavPlaying()
  
  Returns true when a file is playing.

* wavStopPlaying()
  
  Stops playing the current file.

* wavLoop()
  
  Must be called during loop to make sure the play buffer is kept full.

* showDir()
  
  Show details about the files stored on SPIFFS

