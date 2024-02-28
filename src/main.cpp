#include <Arduino.h>
#include "display.h"
#include "encoder.h"

void setup() {
  // Start the serial monitor to show output
  Serial.begin(115200);

  // Display pins
  pinMode(LCD_BL, OUTPUT);

  // Set encoder pins and attach interrupts
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC_A), read_encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), read_encoder, CHANGE);

  tft.setFont(&FreeMonoBold18pt7b);
  tft.begin();
  tft.fillScreen(GC9A01A_BLACK);
  wakeDisplay(0);
  writeText();
  // delay(5000);
  // sleepDisplay(0);
  // fillArc(120, 120, 270, 12, 122, 122, 22, GC9A01A_YELLOW);
  
  for (int n = 0; n < 61; n++) {
    fillArc(120, 120, 269, n, 122, 122, 24, GC9A01A_YELLOW);
    delay(16);
  }
}

void loop() {
  static int lastCounter = 0;

  // If count has changed print the new value to serial
  if(counter != lastCounter){
    Serial.println(counter);
    lastCounter = counter;
  }


}

