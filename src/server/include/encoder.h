// Based on https://github.com/mo-thunderz/RotaryEncoder/blob/main/Arduino/ArduinoRotaryEncoder/ArduinoRotaryEncoder.ino

#ifndef ENCODERS_H
#define ENCODERS_H

// SHARED - Buttons share the same pins
#define BRIGHTNESS_BTN 35
#define SPECTRUM_BTN 32

// Define rotary encoder pins

// IN CABIN
#define BRIGHTNESS_ENC_CAB_A 12
#define BRIGHTNESS_ENC_CAB_B 14

#define SPECTRUM_ENC_CAB_A 26
#define SPECTRUM_ENC_CAB_B 25
 
// IN FRIDGE/DOOR
#define BRIGHTNESS_ENC_DOOR_A 27
#define BRIGHTNESS_ENC_DOOR_B 33

#define SPECTRUM_ENC_DOOR_A 15
#define SPECTRUM_ENC_DOOR_B 2

#define _pauseLength 25000  
#define _fastIncrement 10 
extern volatile int rawBrightnessVal;
extern volatile int rawSpectrumVal;

int readBrightnessEncoderCab(); 
int readBrightnessEncoderDoor(); 

int readSpectrumEncoderCab();
int readSpectrumEncoderDoor(); 

#endif
