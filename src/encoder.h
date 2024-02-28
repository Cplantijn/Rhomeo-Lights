#ifndef ENCODERS_H
#define ENCODERS_H

// Define rotary encoder pins
#define ENC_A 16
#define ENC_B 19

#define _pauseLength 25000  
#define _fastIncrement 10 
extern volatile int counter;

// From https://github.com/mo-thunderz/RotaryEncoder/blob/main/Arduino/ArduinoRotaryEncoder/ArduinoRotaryEncoder.ino
void read_encoder(); 

#endif
