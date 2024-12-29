// #include <Arduino.h>
// #include <Wire.h>
// #include <SPI.h>
// #include "FastLED.h"
// #include "FastLED_RGBW.h"
// #include "leds.h"

// struct Color {
//   int r;
//   int g;
//   int b;
//   int w;
// };

// Color colors[] = {
//   {255, 0, 0, 0},    // Red
//   {0, 255, 0, 0},    // Green 
//   {0, 0, 255, 0},    // Blue
//   {255, 165, 0, 0},  // Orange
//   {255, 0, 255, 0},   // Magenta
//   {0, 0, 0, 255}     // White
// };


// CRGBW awningLeds[AWNING_NUM_LEDS];
// CRGB *awningLedsRGB = (CRGB *) &awningLeds[0];

// void setup() {
//   Serial.begin(115200);
//   FastLED.addLeds<WS2812B, AWNING_DATA_PIN, RGB>(awningLedsRGB, getRGBWsize(AWNING_NUM_LEDS));
//   FastLED.clear();

//   FastLED.setBrightness(255);

//   for (int i = 0; i < AWNING_NUM_LEDS; i++) {
//     awningLeds[i] = CRGBW(0, 0, 0, 100);

//     // int scaledRed = round(currentColor.r * (currentSelectorState.brightness / 100.0));
//     //   int scaledGreen = round(currentColor.g * (currentSelectorState.brightness / 100.0));
//     //   int scaledBlue = round(currentColor.b * (currentSelectorState.brightness / 100.0));
//       // awningLeds[i] = CRGBW(scaledRed, scaledGreen, scaledBlue, 0);

//   }


//   FastLED.show();
  
// }

// void loop() {
//   int numLeds = FastLED.size();
//   static int colorIndex = 0;
  
//   for (int i = 0; i < AWNING_NUM_LEDS; i++) {
//     Color currentColor = colors[colorIndex];
//     awningLeds[i] = CRGBW(currentColor.r, currentColor.g, currentColor.b, currentColor.w);
//   }

//   colorIndex = (colorIndex + 1) % (sizeof(colors) / sizeof(colors[0]));
//   Serial.println("colorIndex: " + String(colorIndex));
//   FastLED.show();

//   delay(1000); // Add delay to make color changes visible
// }

