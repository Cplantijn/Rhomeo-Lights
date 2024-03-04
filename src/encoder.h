// Based on https://github.com/mo-thunderz/RotaryEncoder/blob/main/Arduino/ArduinoRotaryEncoder/ArduinoRotaryEncoder.ino

#ifndef ENCODERS_H
#define ENCODERS_H

// Define rotary encoder pins
#define BRIGHTNESS_ENC_A 15
#define BRIGHTNESS_ENC_B 2

#define SPECTRUM_ENC_A 17
#define SPECTRUM_ENC_B 5

#define _pauseLength 25000  
#define _fastIncrement 10 
extern volatile int rawBrightnessVal;
extern volatile int rawSpectrumVal;

int readBrightnessEncoder(); 
int readSpectrumEncoder();

#endif
