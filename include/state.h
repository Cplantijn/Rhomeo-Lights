#ifndef STATE_H
#define STATE_H
#include <string>
#include "selector.h"

struct SelectorState {
  char selectorName[9]; // 8 characters + null terminator
  int brightness; 
  int spectrum;
  bool isWhite;
};

extern SelectorState currentState[NUM_SELECTORS]; 
extern char currentSelector[9];

const SelectorState& getStateForSelector(const char* selectorName);
void cycleCurrentSelector();
void setSelector(const char* selectorName);
void incrementBrightness(const char* selectorName, int newBrightness);
void setBrightness(const char* selectorName, int value);
void incrementSpectrum(const char* selectorName, int newSpectrum);
void setWhite(const char* selectorName, bool isWhite);
void initializeStates();

#endif 