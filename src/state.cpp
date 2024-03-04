#include "selector.h"
#include "state.h"
#include <cstring> // Include for string functions
#include "spectrum.h"

SelectorState currentState[NUM_SELECTORS]; 
char currentSelector[9];

void initializeStates() {
  for (size_t i = 0; i < NUM_SELECTORS; ++i) { // Use NUM_SELECTORS for consistency
    strncpy(currentState[i].selectorName, SELECTORS[i], 8); 
    currentState[i].selectorName[8] = '\0'; // Ensure null termination
    currentState[i].state.brightness = 0; 
    currentState[i].state.spectrum = 0;   
  }

  strncpy(currentSelector, SELECTORS[0], 8);
  currentSelector[8] = '\0';
}

const SelectorState& getStateForSelector(const char* selectorName) {
  for (size_t i = 0; i < NUM_SELECTORS; ++i) { 
    if (strcmp(currentState[i].selectorName, selectorName) == 0) { 
      return currentState[i];
    }
  }

  throw std::runtime_error("Selector not found: " + std::string(selectorName)); 
}

void updateBrightness(const char* selectorName, int deltaChange) {
  for (size_t i = 0; i < NUM_SELECTORS; ++i) { 
    if (strcmp(currentState[i].selectorName, selectorName) == 0) {
      int nextBrightness = currentState[i].state.brightness + deltaChange;
      currentState[i].state.brightness = std::max(0, std::min(100, nextBrightness));
      return;
    }
  }
}

void updateSpectrum(const char* selectorName, int deltaChange) {
  for (size_t i = 0; i < NUM_SELECTORS; ++i) { 
    if (strcmp(currentState[i].selectorName, selectorName) == 0) {
      int nextSpectrum = currentState[i].state.spectrum + deltaChange;
      currentState[i].state.spectrum = std::max(0, std::min(int(SPECTRUM_STEPS), nextSpectrum));
      return;
    }
  }
}

void cycleCurrentSelector() {
  size_t currentIndex = 0;

  for (size_t i = 0; i < NUM_SELECTORS; ++i) {
    if (strcmp(SELECTORS[i], currentSelector) == 0) { // Use strcmp for C-style strings 
      currentIndex = i;
      break;
    }
  }

  size_t nextIndex = (currentIndex + 1) % NUM_SELECTORS;
  strncpy(currentSelector, SELECTORS[nextIndex], 8); 
  currentSelector[8] = '\0';
}