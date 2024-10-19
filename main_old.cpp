#include <Arduino.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <ButtonDebounce.h>
#include <cmath>

#include "display.h"
#include "util.h"
#include "encoder.h"
#include "spectrum.h"
#include "selector.h"
#include "leds.h"
#include "state.h"
#include "FastLED.h"
#include "FastLED_RGBW.h"

volatile bool brightnessPressed = false;
volatile bool spectrumPressed = false;
volatile bool selectorPressed = false;

// Timeouts and debounces
const int encWaitTime = 50;

unsigned long lastBrightnessInteraction = 0;
unsigned long lastSpectrumInteraction = 0;

CRGBW cabinLeds[CABIN_NUM_LEDS];
CRGB *cabinLedsRGB = (CRGB *) &cabinLeds[0];

CRGBW awningLeds[AWNING_NUM_LEDS];
CRGB *awningLedsRGB = (CRGB *) &awningLeds[0];
std::vector<Color> spectrumSteps;

ButtonDebounce selectorBtn(SELECTOR_BUTTON, 50);
ButtonDebounce brightnessBtn(BRIGHTNESS_BTN, 50);
ButtonDebounce spectrumBtn(SPECTRUM_BTN, 50);

void onSelectorButtonPress(const int state){
  if (state == LOW) {
    selectorPressed = true;
    cycleCurrentSelector();
    Serial.println("SELECTOR pressed, Mode is now: " + String(currentSelector));
  }
}

void onBrightnessButtonPress(const int state){
  if (state == LOW) {
    SelectorState currentSelectorState = getStateForSelector(currentSelector);
    brightnessPressed = true;

    if (currentSelectorState.brightness > 0) {
      setBrightness(currentSelector, 0);
    } else {
      setBrightness(currentSelector, 5);
    }
  }
}

void onSpectrumButtonPress(const int state){
  if (state == LOW) {
    spectrumPressed = true;
  }
}

void onBrightnessChangeCab() {
  unsigned long currentTime = millis();

  if (currentTime - lastBrightnessInteraction > encWaitTime) {
    lastBrightnessInteraction = currentTime;
    return;
  }

  lastBrightnessInteraction = currentTime;

  int changeVal = readBrightnessEncoderCab();
  incrementBrightness(currentSelector, changeVal);
}


void onBrightnessChangeDoor() {
  unsigned long currentTime = millis();

  if (currentTime - lastBrightnessInteraction > encWaitTime) {
    lastBrightnessInteraction = currentTime;
    return;
  }

  lastBrightnessInteraction = currentTime;
  int changeVal = readBrightnessEncoderDoor();
  incrementBrightness(currentSelector, changeVal);
}

// void onSpectrumChangeCab() {
//   unsigned long currentTime = millis();
//   // Debouncing the encoder
//   if (currentTime - lastSpectrumInteraction > encWaitTime) {
//     lastSpectrumInteraction = currentTime;
//     return;
//   }

//   lastSpectrumInteraction = currentTime;
//   int changeVal = readSpectrumEncoderCab();
//   incrementSpectrum(currentSelector, changeVal);
// }

// void onSpectrumChangeDoor() {
//   unsigned long currentTime = millis();
//   // Debouncing the encoder
//   if (currentTime - lastSpectrumInteraction > encWaitTime) {
//     lastSpectrumInteraction = currentTime;
//     return;
//   }

//   lastSpectrumInteraction = currentTime;
//   int changeVal = readSpectrumEncoderDoor();
//   incrementSpectrum(currentSelector, changeVal);
// }


void setup() {
  Serial.begin(115200);

  // ----------------- SHARED -----------------
  pinMode(SELECTOR_BUTTON, INPUT_PULLUP);
  pinMode(BRIGHTNESS_BTN, INPUT_PULLUP);
  pinMode(SPECTRUM_BTN, INPUT_PULLUP);

  selectorBtn.setCallback(onSelectorButtonPress);
  brightnessBtn.setCallback(onBrightnessButtonPress);
  // spectrumBtn.setCallback(onSpectrumButtonPress);
  // ----------------- CABIN -----------------

  pinMode(BRIGHTNESS_ENC_CAB_A, INPUT_PULLUP);
  pinMode(BRIGHTNESS_ENC_CAB_B, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BRIGHTNESS_ENC_CAB_A), onBrightnessChangeCab, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BRIGHTNESS_ENC_CAB_B), onBrightnessChangeCab, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(SPECTRUM_ENC_CAB_A), onSpectrumChangeCab, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(SPECTRUM_ENC_CAB_B), onSpectrumChangeCab, CHANGE);

  // ----------------- DOOR -----------------
  pinMode(BRIGHTNESS_ENC_DOOR_A, INPUT_PULLUP);
  pinMode(BRIGHTNESS_ENC_DOOR_B, INPUT_PULLUP);

  // pinMode(SPECTRUM_ENC_DOOR_A, INPUT_PULLUP);
  // pinMode(SPECTRUM_ENC_DOOR_B, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BRIGHTNESS_ENC_DOOR_A), onBrightnessChangeDoor, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BRIGHTNESS_ENC_DOOR_B), onBrightnessChangeDoor, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(SPECTRUM_ENC_DOOR_A), onSpectrumChangeDoor, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(SPECTRUM_ENC_DOOR_B), onSpectrumChangeDoor, CHANGE);

  // // ----------------- STATE -----------------
  spectrumSteps = generateSpectrumSteps(SPECTRUM_STEPS);
  initializeStates();
  
  // ----------------- DISPLAY -----------------
  pinMode(LCD_BL, OUTPUT);
  tftCab.begin();
  tftCab.fillScreen(GC9A01A_YELLOW);
  tftCab.setFont(&FreeMonoBold18pt7b);
  tftDoor.begin();
  tftDoor.fillScreen(GC9A01A_RED);
  tftDoor.setFont(&FreeMonoBold18pt7b);  
  setDisplaysToActive();
  // writeSelector(currentSelector);

  // ----------------- LEDS -----------------
  FastLED.addLeds<WS2812B, CABIN_DATA_PIN, RGB>(cabinLedsRGB, getRGBWsize(CABIN_NUM_LEDS));
  FastLED.addLeds<WS2812B, AWNING_DATA_PIN, RGB>(awningLedsRGB, getRGBWsize(AWNING_NUM_LEDS));
  FastLED.clear();
  int numLeds = FastLED.size();
  FastLED.setBrightness(255);
  FastLED.show();
}

void loop() {
  // selectorBtn.update();
  // brightnessBtn.update();
  // // spectrumBtn.update();

  // if (brightnessPressed) {
  //   brightnessPressed = false;
  //   Serial.println("Brightness pressed");
  // }

  // if (selectorPressed) {
  //   selectorPressed = false;
  // }

  // // if (spectrumPressed) {
  // //   setWhite(currentSelector, true);
  // //   spectrumPressed = false;
  // // }

  // static long idleTimeoutStartTime = 0;
  // static int lastRawBrightnessVal = 0;
  // static int lastRawSpectrumVal = 0;
  // static int lastBrightnessVal = -1;
  // static int lastSpectrumVal = -1;
  // static int lastIsWhiteVal = false;
  // static char lastSelector[9];

  // bool hasInputsChanged = false;

  // if (strcmp(currentSelector, lastSelector) != 0) {
  //   strncpy(lastSelector, currentSelector, 8);
  //   lastSelector[8] = '\0';
  //   writeSelector(currentSelector);
  //   hasInputsChanged = true;
  // }

  // SelectorState currentSelectorState = getStateForSelector(currentSelector);

  // if (rawBrightnessVal != lastRawBrightnessVal) {
  //   lastRawBrightnessVal = rawBrightnessVal;
  //   hasInputsChanged = true;
  // }

  // if (rawSpectrumVal != lastRawSpectrumVal) {
  //   lastRawSpectrumVal = rawSpectrumVal;
  //   hasInputsChanged = true;
  // }

  // if (
  //   lastIsWhiteVal != currentSelectorState.isWhite
  //   || lastSpectrumVal != currentSelectorState.spectrum
  //   || lastBrightnessVal != currentSelectorState.brightness) {
  //   hasInputsChanged = true;
  // }

  // if (hasInputsChanged) {
  //   isIdleTimeoutActive = false;
  //   if (isDisplaysIdle) {
        // Serial.println("1,1");
  //     setDisplaysToActive();
  //   }
  // }
  
  // if (currentSelectorState.brightness != lastBrightnessVal || currentSelectorState.spectrum != lastSpectrumVal || lastIsWhiteVal != currentSelectorState.isWhite) {
  //   // Serial.println("Selector: " + String(currentSelector));
  //   // Serial.println("Brightness: " + String(currentSelectorState.brightness) + " Spectrum: " + String(currentSelectorState.spectrum) + " IsWhite: " + String(currentSelectorState.isWhite));
  //   // Serial.println("Last brightness: " + String(lastBrightnessVal) + " Last spectrum: " + String(lastSpectrumVal) + " Last isWhite: " + String(lastIsWhiteVal) + "\n");

  //   lastBrightnessVal = currentSelectorState.brightness;
  //   lastSpectrumVal = currentSelectorState.spectrum;
  //   lastIsWhiteVal = currentSelectorState.isWhite;
  //   bool isWhite = currentSelectorState.isWhite;
  //   Color currentColor;

  //   if (isWhite) {
  //     currentColor.r = 255;
  //     currentColor.g = 255;
  //     currentColor.b = 255;
  //   } else {
  //     currentColor = spectrumSteps[currentSelectorState.spectrum];
  //   }

    // int tftColor = colorTo565(currentColor);
    // String colorMessage = "0," + currentSelector + "," + String(currentColor.r) + "," + String(currentColor.g) + "," + String(currentColor.b) + "," + String(currentSelectorState.brightness);
    // Serial.println(colorMessage);
  //   // Draw the arc
  //   int arcSegments = normalizeRangeInt(currentSelectorState.brightness, 0, 100, 0, 91);

  //   fillArc(269, arcSegments, 24, tftColor);
  //   writePercentage(currentSelectorState.brightness, tftColor);
    
  //   if (strcmp(currentSelector, "Cabin") == 0) {
  //     if (isWhite) {
  //       for (int i = 0; i < CABIN_NUM_LEDS; i++) {
  //         cabinLeds[i] = CRGBW(0, 0, 0, round((currentSelectorState.brightness / 100.0) * 255));
  //       }
  //     } else {
  //       // Scale the color based on the brightness (0-100%
  //       int scaledRed = round(currentColor.r * (currentSelectorState.brightness / 100.0));
  //       int scaledGreen = round(currentColor.g * (currentSelectorState.brightness / 100.0));
  //       int scaledBlue = round(currentColor.b * (currentSelectorState.brightness / 100.0));

  //       for(int i = 0; i < CABIN_NUM_LEDS; i++){
  //         cabinLeds[i] = CRGBW(scaledRed, scaledGreen, scaledBlue, 0);
  //       }
  //     }
  //     FastLED.show();
  //   }

  //   if (strcmp(currentSelector, "Awning") == 0) {
  //     if (isWhite) {
  //       for(int i = 0; i < AWNING_NUM_LEDS; i++){
  //         awningLeds[i] = CRGBW(0, 0, 0, round((currentSelectorState.brightness / 100.0) * 255));
  //       }
  //     } else {
  //       // Scale the color based on the brightness (0-100%
  //       int scaledRed = round(currentColor.r * (currentSelectorState.brightness / 100.0));
  //       int scaledGreen = round(currentColor.g * (currentSelectorState.brightness / 100.0));
  //       int scaledBlue = round(currentColor.b * (currentSelectorState.brightness / 100.0));

  //       for(int i = 0; i < AWNING_NUM_LEDS; i++){
  //         awningLeds[i] = CRGBW(scaledRed, scaledGreen, scaledBlue, 0);
  //       }
  //     }

  //     FastLED.show();
  //   }
  // }
  
  // if (!isIdleTimeoutActive) {
  //   idleTimeoutStartTime = millis();
  //   isIdleTimeoutActive = true;
  // }

  // if (isIdleTimeoutActive && !isDisplaysIdle && (millis() - idleTimeoutStartTime >= IDLE_TIMEOUT_MS)) {
  //   setDisplaysToIdle();
  //   Serial.println("1,0");
  //   setSelector(STR_CABIN);
  //   isIdleTimeoutActive = false;
  // }
}

