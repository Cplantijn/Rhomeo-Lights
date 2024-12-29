#include <Arduino.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <ButtonDebounce.h>
#include <cmath>

#include <display.h>
#include <util.h>
#include <spectrum.h>
#include <selector.h>
#include "encoder.h"
#include "leds.h"
#include "state.h"
#include "FastLED.h"
#include "FastLED_RGBW.h"

volatile bool brightnessPressed = false;
volatile bool spectrumPressed = false;
volatile bool selectorPressed = false;

CRGBW cabinLeds[CABIN_NUM_LEDS];
CRGB *cabinLedsRGB = (CRGB *) &cabinLeds[0];

CRGBW awningLeds[AWNING_NUM_LEDS];
CRGB *awningLedsRGB = (CRGB *) &awningLeds[0];
std::vector<Color> spectrumSteps;

ButtonDebounce selectorBtn(SELECTOR_BUTTON, 50);
ButtonDebounce brightnessBtn(BRIGHTNESS_BTN, 50);
ButtonDebounce spectrumBtn(SPECTRUM_BTN, 50);

// The van is an electrically noisy environment, especially around
// the kitchen area where fans, coffee makers, microwaves, stove pezio's
// etc are used. This interrupt handler is used to debounce the encoder
// interrupts. Movement is only considered if it has been proceeded by another
// movement 20ms prior. Every 2 seconds things reset.
volatile unsigned long lastMovementTime = 0;
const unsigned long debounceDelay = 20; // 20ms debounce delay
const unsigned long inactivityThreshold = 2000; //


void onSelectorButtonPress(const int state){
  if (state == LOW) {
    selectorPressed = true;
    cycleCurrentSelector();
    Serial.println("SELECTOR pressed, Mode is now: " + String(currentSelector));
  }
}

// void onBrightnessButtonPress(const int state){
//   if (state == LOW) {
//     SelectorState currentSelectorState = getStateForSelector(currentSelector);
//     brightnessPressed = true;

//     if (currentSelectorState.brightness > 0) {
//       setBrightness(currentSelector, 0);
//     } else {
//       setBrightness(currentSelector, 5);
//     }
//   }
// }

// void onSpectrumButtonPress(const int state){
//   if (state == LOW) {
//     spectrumPressed = true;
//   }
// }

void onBrightnessChangeCab() {
  unsigned long currentTime = millis();
  
  if ((currentTime - lastMovementTime) > inactivityThreshold) {
    // Reset debounce timer if inactive for more than 2 seconds
    lastMovementTime = currentTime;
  }

  if ((currentTime - lastMovementTime) > debounceDelay) {
    int changeVal = readBrightnessEncoderCab();
    incrementBrightness(currentSelector, changeVal);
    return;
  }
}


void onBrightnessChangeDoor() {
  unsigned long currentTime = millis();
  if ((currentTime - lastMovementTime) > inactivityThreshold) {
    // Reset debounce timer if inactive for more than 2 seconds
    lastMovementTime = currentTime;
  }

  if ((currentTime - lastMovementTime) > debounceDelay) {
    int changeVal = readBrightnessEncoderDoor();
    incrementBrightness(currentSelector, changeVal);
  }
}

// void onSpectrumChangeCab() {
//   int changeVal = readSpectrumEncoderCab();
//   incrementSpectrum(currentSelector, changeVal);
// }

// void onSpectrumChangeDoor() {
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
  // brightnessBtn.setCallback(onBrightnessButtonPress);
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

  pinMode(SPECTRUM_ENC_DOOR_A, INPUT_PULLUP);
  pinMode(SPECTRUM_ENC_DOOR_B, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BRIGHTNESS_ENC_DOOR_A), onBrightnessChangeDoor, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BRIGHTNESS_ENC_DOOR_B), onBrightnessChangeDoor, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(SPECTRUM_ENC_DOOR_A), onSpectrumChangeDoor, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(SPECTRUM_ENC_DOOR_B), onSpectrumChangeDoor, CHANGE);

  // // ----------------- STATE -----------------
  initializeStates();
  
  // ----------------- DISPLAY -----------------
  pinMode(LCD_BL, OUTPUT);
  tft.begin();
  tft.fillScreen(GC9A01A_BLACK);
  tft.setFont(&FreeMonoBold18pt7b);  
  setDisplaysToActive();
  writeSelector(currentSelector);

  // ----------------- LEDS -----------------
  FastLED.addLeds<WS2812B, CABIN_DATA_PIN, RGB>(cabinLedsRGB, getRGBWsize(CABIN_NUM_LEDS));
  FastLED.addLeds<WS2812B, AWNING_DATA_PIN, RGB>(awningLedsRGB, getRGBWsize(AWNING_NUM_LEDS));
  FastLED.clear();

  FastLED.setBrightness(255);
  FastLED.show();
}

void loop() {
  selectorBtn.update();
  brightnessBtn.update();
  spectrumBtn.update();

  if (brightnessPressed) {
    brightnessPressed = false;
    Serial.println("Brightness pressed");
  }

  if (selectorPressed) {
    selectorPressed = false;
  }

  if (spectrumPressed) {
    setWhite(currentSelector, true);
    spectrumPressed = false;
  }

  static long idleTimeoutStartTime = 0;
  static int lastRawBrightnessVal = 0;
  static int lastRawSpectrumVal = 0;
  static int lastBrightnessVal = -1;
  static int lastSpectrumVal = -1;
  static int lastIsWhiteVal = false;
  static char lastSelector[9];

  bool hasInputsChanged = false;

  if (strcmp(currentSelector, lastSelector) != 0) {
    strncpy(lastSelector, currentSelector, 8);
    lastSelector[8] = '\0';
    writeSelector(currentSelector);
    hasInputsChanged = true;
  }

  SelectorState currentSelectorState = getStateForSelector(currentSelector);

  if (rawBrightnessVal != lastRawBrightnessVal) {
    lastRawBrightnessVal = rawBrightnessVal;
    hasInputsChanged = true;
  }

  if (rawSpectrumVal != lastRawSpectrumVal) {
    lastRawSpectrumVal = rawSpectrumVal;
    hasInputsChanged = true;
  }

  if (
    lastIsWhiteVal != currentSelectorState.isWhite
    || lastSpectrumVal != currentSelectorState.spectrum
    || lastBrightnessVal != currentSelectorState.brightness) {
    hasInputsChanged = true;
  }

  if (hasInputsChanged) {
    isIdleTimeoutActive = false;

    if (isDisplaysIdle) {
      setDisplaysToActive();
      Serial.println("1,1");
    }
  }
  
  if (currentSelectorState.brightness != lastBrightnessVal || currentSelectorState.spectrum != lastSpectrumVal || lastIsWhiteVal != currentSelectorState.isWhite) {
    // Serial.println("Selector: " + String(currentSelector));
    // Serial.println("Brightness: " + String(currentSelectorState.brightness) + " Spectrum: " + String(currentSelectorState.spectrum) + " IsWhite: " + String(currentSelectorState.isWhite));
    // Serial.println("Last brightness: " + String(lastBrightnessVal) + " Last spectrum: " + String(lastSpectrumVal) + " Last isWhite: " + String(lastIsWhiteVal) + "\n");

    lastBrightnessVal = currentSelectorState.brightness;
    lastSpectrumVal = currentSelectorState.spectrum;
    lastIsWhiteVal = currentSelectorState.isWhite;
    bool isWhite = currentSelectorState.isWhite;
    Color currentColor;

    if (isWhite) {
      currentColor.r = 255;
      currentColor.g = 255;
      currentColor.b = 255;
    } else {
      currentColor = SPECTRUM_COLORS[currentSelectorState.spectrum];
    }

    int tftColor = colorTo565(currentColor);
    // Draw the arc
    int arcSegments = normalizeRangeInt(currentSelectorState.brightness, 0, 100, 0, 91);

    fillArc(269, arcSegments, 24, tftColor);
    writePercentage(currentSelectorState.brightness, tftColor);
    
    if (strcmp(currentSelector, "Cabin") == 0) {
      if (isWhite) {
        for (int i = 0; i < CABIN_NUM_LEDS; i++) {
          cabinLeds[i] = CRGBW(0, 0, 0, round((currentSelectorState.brightness / 100.0) * 255));
        }
      } else {
        // Scale the color based on the brightness (0-100%
        int scaledRed = round(currentColor.r * (currentSelectorState.brightness / 100.0));
        int scaledGreen = round(currentColor.g * (currentSelectorState.brightness / 100.0));
        int scaledBlue = round(currentColor.b * (currentSelectorState.brightness / 100.0));

        for(int i = 0; i < CABIN_NUM_LEDS; i++){
          cabinLeds[i] = CRGBW(scaledRed, scaledGreen, scaledBlue, 0);
        }
      }
      FastLED.show();
    }

    if (strcmp(currentSelector, "Awning") == 0) {
      if (isWhite) {
        for(int i = 0; i < AWNING_NUM_LEDS; i++){
          awningLeds[i] = CRGBW(0, 0, 0, round((currentSelectorState.brightness / 100.0) * 255));
        }
      } else {
        // Scale the color based on the brightness (0-100%
        int scaledRed = round(currentColor.r * (currentSelectorState.brightness / 100.0));
        int scaledGreen = round(currentColor.g * (currentSelectorState.brightness / 100.0));
        int scaledBlue = round(currentColor.b * (currentSelectorState.brightness / 100.0));

        for(int i = 0; i < AWNING_NUM_LEDS; i++){
          awningLeds[i] = CRGBW(scaledRed, scaledGreen, scaledBlue, 0);
        }
      }

      FastLED.show();
    }
  }
  
  if (hasInputsChanged) {
    bool isWhite = currentSelectorState.isWhite;
    Color currentColor;

    if (isWhite) {
      currentColor.r = 255;
      currentColor.g = 255;
      currentColor.b = 255;
    } else {
      currentColor = SPECTRUM_COLORS[currentSelectorState.spectrum];
    }

    String colorMessage = "0," + String(currentSelector) + "," + String(currentColor.r) + "," + String(currentColor.g) + "," + String(currentColor.b) + "," + String(currentSelectorState.brightness);
    Serial.println(colorMessage);
  }

  if (!isIdleTimeoutActive) {
    idleTimeoutStartTime = millis();
    isIdleTimeoutActive = true;
  }

  if (isIdleTimeoutActive && !isDisplaysIdle && (millis() - idleTimeoutStartTime >= IDLE_TIMEOUT_MS)) {
    setDisplaysToIdle();
    setSelector(STR_CABIN);
    isIdleTimeoutActive = false;
    Serial.println("1,0");
  }
}

