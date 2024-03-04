#include <Arduino.h>
#include "display.h"
#include "util.h"
#include "encoder.h"
#include "spectrum.h"
#include "selector.h"
#include "state.h"

#include <Fonts/FreeMonoBold18pt7b.h>
#include <ButtonDebounce.h>

std::vector<Color> spectrumSteps;

unsigned long previousMillis = 0; 
const long interval = 10000; // Interval for printing heap statistics (in milliseconds)

ButtonDebounce buttonA(SELECTOR_BUTTON_A, 50);

void buttonChanged(const int state){
  if (state == LOW) {
    cycleCurrentSelector();
  }
}

void onBrightnessChange() {
  int changeVal = readBrightnessEncoder();
  updateBrightness(currentSelector, changeVal);
}

void onSpectrumChange() {
  int changeVal = readSpectrumEncoder();
  updateSpectrum(currentSelector, changeVal);
}

void setup() {
  // Start the serial monitor to show output
  Serial.begin(115200);

  // Selector button
  buttonA.setCallback(buttonChanged);

  // Display pins
  pinMode(LCD_BL, OUTPUT);

  // Set encoder pins and attach interrupts

  // Brightness encoders
  pinMode(BRIGHTNESS_ENC_A, INPUT_PULLUP);
  pinMode(BRIGHTNESS_ENC_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BRIGHTNESS_ENC_A), onBrightnessChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BRIGHTNESS_ENC_B), onBrightnessChange, CHANGE);

  // Spectrum encoders
  pinMode(SPECTRUM_ENC_A, INPUT_PULLUP);
  pinMode(SPECTRUM_ENC_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SPECTRUM_ENC_A), onSpectrumChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(SPECTRUM_ENC_B), onSpectrumChange, CHANGE);

  spectrumSteps = generateSpectrumSteps(SPECTRUM_STEPS);
  initializeStates();
  tft.begin();
  tft.fillScreen(GC9A01A_BLACK);
  tft.setFont(&FreeMonoBold18pt7b);
  setDisplaysToActive();
  writeSelector(currentSelector);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.print("Free heap: ");
    Serial.println(ESP.getFreeHeap()); 
  }

  buttonA.update();
  static int lastRawBrightnessVal = 0;
  static int lastRawSpectrumVal = 0;
  static int lastBrightnessVal = -1;
  static int lastSpectrumVal = -1;
  static char lastSelector[9];

  bool hasRawChange = false;

  if (strcmp(currentSelector, lastSelector) != 0) {
    strncpy(lastSelector, currentSelector, 8);
    lastSelector[8] = '\0';
    writeSelector(currentSelector);
    hasRawChange = true;
  }

  SelectorState currentState = getStateForSelector(currentSelector);

  if (rawBrightnessVal != lastRawBrightnessVal) {
    hasRawChange = true;
  }

  if (rawSpectrumVal != lastRawSpectrumVal) {
    lastRawSpectrumVal = rawSpectrumVal;
    hasRawChange = true;
  }

  if (hasRawChange) {
    isIdleTimeoutActive = false;
    if (isDisplaysIdle) {
      setDisplaysToActive();
    }
  }
  
  if (currentState.state.brightness != lastBrightnessVal || currentState.state.spectrum != lastSpectrumVal) {
    lastBrightnessVal = currentState.state.brightness;
    lastSpectrumVal = currentState.state.spectrum;

    Color color = spectrumSteps[currentState.state.spectrum];
    String colorStr = String(color.r) + ", " + String(color.g) + ", " + String(color.b);
    int tftColor = colorTo565(color);

    // Draw the arc
    int arcSegments = normalizeRangeInt(currentState.state.brightness, 0, 100, 0, 91);

    fillArc(269, arcSegments, 24, tftColor);
    writePercentage(currentState.state.brightness, tftColor);
  }
  
  if (!isIdleTimeoutActive) {
    idleTimeoutStartTime = millis();
    isIdleTimeoutActive = true;
  }

  if (isIdleTimeoutActive && (millis() - idleTimeoutStartTime >= IDLE_TIMEOUT_MS)) {
    setDisplaysToIdle();
    isIdleTimeoutActive = false;

    // strncpy(currentSelector, SELECTORS[0], 8);
    // currentSelector[8] = '\0';
  }
}

