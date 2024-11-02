#include <selector.h>
#include <Arduino.h>
#include "state.h"
#include <cstring> // Include for string functions
#include <spectrum.h>

SelectorState currentState[NUM_SELECTORS]; 
char currentSelector[9];

void initializeStates() {
  for (size_t i = 0; i < NUM_SELECTORS; ++i) { // Use NUM_SELECTORS for consistency
    strncpy(currentState[i].selectorName, SELECTORS[i], 8); 
    currentState[i].selectorName[8] = '\0'; // Ensure null termination
    currentState[i].brightness = 0; 
    currentState[i].spectrum = 0;
    currentState[i].isWhite = true;
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

void incrementBrightness(const char* selectorName, int deltaChange) {
  for (size_t i = 0; i < NUM_SELECTORS; ++i) { 
    if (strcmp(currentState[i].selectorName, selectorName) == 0) {
      int nextBrightness = currentState[i].brightness + deltaChange;
      currentState[i].brightness = std::max(0, std::min(100, nextBrightness));
      return;
    }
  }
}

void setBrightness(const char* selectorName, int value) {
  for (size_t i = 0; i < NUM_SELECTORS; ++i) { 
    if (strcmp(currentState[i].selectorName, selectorName) == 0) {
      currentState[i].brightness = value;
      return;
    }
  }
}

void setWhite(const char* selectorName, bool isWhite) {
  for (size_t i = 0; i < NUM_SELECTORS; ++i) { 
    if (strcmp(currentState[i].selectorName, selectorName) == 0) {
      currentState[i].isWhite = isWhite;
      return;
    }
  }
}

void incrementSpectrum(const char* selectorName, int deltaChange) {
  for (size_t i = 0; i < NUM_SELECTORS; ++i) { 
    if (strcmp(currentState[i].selectorName, selectorName) == 0) {
      int newSpectrum = (currentState[i].spectrum + deltaChange) % SPECTRUM_STEPS;
      
      if (newSpectrum < 0) {
        newSpectrum += SPECTRUM_STEPS; 
      }
      
      // Convert to 0-19 range
      int spectrumIndex = newSpectrum / (SPECTRUM_STEPS / NUM_SPECTRUM_COLORS);

      if (currentState[i].isWhite) {
        currentState[i].isWhite = false;
      }
      currentState[i].spectrum = spectrumIndex;
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

void setSelector(const char* selectorName) {
  // Safer way to setting selector since it must match element in SELECTORS 
  for (size_t i = 0; i < NUM_SELECTORS; ++i) {
    if (strcmp(SELECTORS[i], selectorName) == 0) {
      strncpy(currentSelector, selectorName, 8);
      currentSelector[8] = '\0';
      return;
    }
  }
}