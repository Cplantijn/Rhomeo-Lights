#ifndef STATE_H
#define STATE_H
#include <string>
#include "selector.h"

struct LightState {
  int brightness; 
  int spectrum;
};

struct SelectorState {
  char selectorName[9]; // 8 characters + null terminator
  LightState state;
};

extern SelectorState currentState[NUM_SELECTORS]; 
extern char currentSelector[9];

const SelectorState& getStateForSelector(const char* selectorName);
void cycleCurrentSelector();
void updateBrightness(const char* selectorName, int newBrightness); 
void updateSpectrum(const char* selectorName, int newSpectrum);
void initializeStates();

#endif 