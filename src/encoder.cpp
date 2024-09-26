#include <Arduino.h>
#include "encoder.h"
#include "spectrum.h"

unsigned long _lastIncReadTime = micros(); 
unsigned long _lastDecReadTime = micros();

volatile int rawBrightnessVal = 0;
volatile int rawSpectrumVal = 0;

static const int8_t enc_states[]  = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0}; // Lookup table

int readBrightnessEncoderDoor() {
  // Encoder interrupt routine for both pins. Updates counter
  // if they are valid and have rotated a full indent
  static uint8_t oldBrightnessAB = 3;  // Lookup table index
  static int8_t brightnessEncVal = 0;   // Encoder value  

  oldBrightnessAB <<=2;  // Remember previous state

  if (digitalRead(BRIGHTNESS_ENC_DOOR_A)) oldBrightnessAB |= 0x02; // Add current state of pin A
  if (digitalRead(BRIGHTNESS_ENC_DOOR_B)) oldBrightnessAB |= 0x01; // Add current state of pin B
  
  brightnessEncVal += enc_states[( oldBrightnessAB & 0x0f )];
  int changeVal = 0;

  // Update counter if encoder has rotated a full indent, that is at least 4 steps
  if (brightnessEncVal > 3 ) {        // Four steps forward
    changeVal = 1;
    
    if((micros() - _lastIncReadTime) < _pauseLength) {
      changeVal = _fastIncrement * changeVal; 
    }
    _lastIncReadTime = micros();
    rawBrightnessVal = rawBrightnessVal + changeVal;        // Update raw counter
    brightnessEncVal = 0;
  }
  else if( brightnessEncVal < -3 ) {        // Four steps backward
    changeVal = -1;
    if((micros() - _lastDecReadTime) < _pauseLength) {
      changeVal = _fastIncrement * changeVal; 
    }
    _lastDecReadTime = micros();
    rawBrightnessVal = rawBrightnessVal + changeVal;        // Update raw counter
    brightnessEncVal = 0;
  }

  return changeVal;
}

int readBrightnessEncoderCab() {
  // Encoder interrupt routine for both pins. Updates counter
  // if they are valid and have rotated a full indent
  static uint8_t oldBrightnessAB = 3;  // Lookup table index
  static int8_t brightnessEncVal = 0;   // Encoder value  

  oldBrightnessAB <<=2;  // Remember previous state

  if (digitalRead(BRIGHTNESS_ENC_CAB_A)) oldBrightnessAB |= 0x02; // Add current state of pin A
  if (digitalRead(BRIGHTNESS_ENC_CAB_B)) oldBrightnessAB |= 0x01; // Add current state of pin B
  
  brightnessEncVal += enc_states[( oldBrightnessAB & 0x0f )];
  int changeVal = 0;

  // Update counter if encoder has rotated a full indent, that is at least 4 steps
  if (brightnessEncVal > 3 ) {        // Four steps forward
    changeVal = 1;
    
    if((micros() - _lastIncReadTime) < _pauseLength) {
      changeVal = _fastIncrement * changeVal; 
    }
    _lastIncReadTime = micros();
    rawBrightnessVal = rawBrightnessVal + changeVal;        // Update raw counter
    brightnessEncVal = 0;
  }
  else if( brightnessEncVal < -3 ) {        // Four steps backward
    changeVal = -1;
    if((micros() - _lastDecReadTime) < _pauseLength) {
      changeVal = _fastIncrement * changeVal; 
    }
    _lastDecReadTime = micros();
    rawBrightnessVal = rawBrightnessVal + changeVal;        // Update raw counter
    brightnessEncVal = 0;
  }

  return changeVal;
}

int readSpectrumEncoderCab() {
  // Encoder interrupt routine for both pins. Updates counter
  // if they are valid and have rotated a full indent
  static uint8_t oldSpectrumAB = 3;  // Lookup table index
  static int8_t oldSpectrumEncVal = 0;   // Encoder value  

  oldSpectrumAB <<=2;  // Remember previous state

  if (digitalRead(SPECTRUM_ENC_CAB_A)) oldSpectrumAB |= 0x02; // Add current state of pin A
  if (digitalRead(SPECTRUM_ENC_CAB_B)) oldSpectrumAB |= 0x01; // Add current state of pin B
  
  oldSpectrumEncVal += enc_states[( oldSpectrumAB & 0x0f )];
  int changeVal = 0;
  // Update counter if encoder has rotated a full indent, that is at least 4 steps
  if( oldSpectrumEncVal > 3 ) {        // Four steps forward
    changeVal = 1;
    if((micros() - _lastIncReadTime) < _pauseLength) {
      changeVal = _fastIncrement * changeVal; 
    }
    _lastIncReadTime = micros();
    rawSpectrumVal = rawSpectrumVal + changeVal;        // Update raw counter
    oldSpectrumEncVal = 0;
  }
  else if( oldSpectrumEncVal < -3 ) {        // Four steps backward
    changeVal = -1;
    if((micros() - _lastDecReadTime) < _pauseLength) {
      changeVal = _fastIncrement * changeVal; 
    }
    _lastDecReadTime = micros();
    rawSpectrumVal = rawSpectrumVal + changeVal;        // Update raw counter
    oldSpectrumEncVal = 0;
  }

  return changeVal;
}

int readSpectrumEncoderDoor() {
  // Encoder interrupt routine for both pins. Updates counter
  // if they are valid and have rotated a full indent
  static uint8_t oldSpectrumAB = 3;  // Lookup table index
  static int8_t oldSpectrumEncVal = 0;   // Encoder value  

  oldSpectrumAB <<=2;  // Remember previous state

  if (digitalRead(SPECTRUM_ENC_DOOR_A)) oldSpectrumAB |= 0x02; // Add current state of pin A
  if (digitalRead(SPECTRUM_ENC_DOOR_B)) oldSpectrumAB |= 0x01; // Add current state of pin B
  
  oldSpectrumEncVal += enc_states[( oldSpectrumAB & 0x0f )];
  int changeVal = 0;
  // Update counter if encoder has rotated a full indent, that is at least 4 steps
  if( oldSpectrumEncVal > 3 ) {        // Four steps forward
    changeVal = 1;
    if((micros() - _lastIncReadTime) < _pauseLength) {
      changeVal = _fastIncrement * changeVal; 
    }
    _lastIncReadTime = micros();
    rawSpectrumVal = rawSpectrumVal + changeVal;        // Update raw counter
    oldSpectrumEncVal = 0;
  }
  else if( oldSpectrumEncVal < -3 ) {        // Four steps backward
    changeVal = -1;
    if((micros() - _lastDecReadTime) < _pauseLength) {
      changeVal = _fastIncrement * changeVal; 
    }
    _lastDecReadTime = micros();
    rawSpectrumVal = rawSpectrumVal + changeVal;        // Update raw counter
    oldSpectrumEncVal = 0;
  }

  return changeVal;
}