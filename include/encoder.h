// Based on https://github.com/mo-thunderz/RotaryEncoder/blob/main/Arduino/ArduinoRotaryEncoder/ArduinoRotaryEncoder.ino

#ifndef ENCODERS_H
#define ENCODERS_H

// Define rotary encoder pins
#define BRIGHTNESS_ENC_A 12
#define BRIGHTNESS_ENC_B 14

#define BRIGHTNESS_ENC_BUTTON 35

#define SPECTRUM_ENC_A 26
#define SPECTRUM_ENC_B 25
 
#define SPECTRUM_ENC_BUTTON 32


#define ENC_WAIT_TIME 50;  // 50ms wait time before reading encoder

#define _pauseLength 25000  
#define _fastIncrement 10 
extern volatile int rawBrightnessVal;
extern volatile int rawSpectrumVal;

int readBrightnessEncoder(); 
int readSpectrumEncoder();

#endif
