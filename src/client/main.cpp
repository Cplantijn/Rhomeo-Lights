#include <Arduino.h>
#include <display.h>
#include <spectrum.h>
#include <selector.h>
#include <util.h>

String incomingData = "";

void writeSelectorToDisplay(String selector) {
  char currentSelector[9];

  if (selector == STR_CABIN) {
    strncpy(currentSelector, SELECTORS[0], 8);
    currentSelector[8] = '\0';
  } else if (selector == STR_AWNING) {
   strncpy(currentSelector, SELECTORS[1], 8);
    currentSelector[8] = '\0';
  }

  writeSelector(currentSelector);
}

void setup() {
  Serial.begin(115200); // Start the serial communication at 115200 baud rate
  tft.begin();
  tft.fillScreen(GC9A01A_BLACK);
  tft.setFont(&FreeMonoBold18pt7b);  
  setDisplaysToActive();
  writeSelectorToDisplay("Cabin");
}

void loop() {
  if (Serial.available()) {
    incomingData = Serial.readStringUntil('\n');

    if (incomingData.indexOf(',') == -1) {
      Serial.println("Unrelated message received, ignoring.");
      return;
    }

    Serial.println("Received message: " + incomingData);

    // Parse the message type
    int commaPos = incomingData.indexOf(',');
    String messageType = incomingData.substring(0, commaPos);

    if (messageType == "0") {
      // Handle the first message type (Selector, Color, Brightness)
      int firstComma = incomingData.indexOf(',', commaPos + 1);
      String selector = incomingData.substring(commaPos + 1, firstComma);

      int secondComma = incomingData.indexOf(',', firstComma + 1);
      uint8_t r = incomingData.substring(firstComma + 1, secondComma).toInt();

      int thirdComma = incomingData.indexOf(',', secondComma + 1);
      uint8_t g = incomingData.substring(secondComma + 1, thirdComma).toInt();

      int fourthComma = incomingData.indexOf(',', thirdComma + 1);
      uint8_t b = incomingData.substring(thirdComma + 1, fourthComma).toInt();

      int brightness = incomingData.substring(fourthComma + 1).toInt();
      
      Color color = {r, g, b};
      int tftColor = colorTo565(color);
      // Draw the arc
      int arcSegments = normalizeRangeInt(brightness, 0, 100, 0, 91);

      fillArc(269, arcSegments, 24, tftColor);
      writePercentage(brightness, tftColor);
      writeSelectorToDisplay(selector);
    } else if (messageType == "1") {
      // Handle the second message type (0 or 1)
      int messageVal = incomingData.substring(commaPos + 1).toInt();

      if (messageVal == 0) {
        setDisplaysToIdle();
      } else if (messageVal == 1) {
        setDisplaysToActive();
      } else {
        Serial.println("Unknown message type received, ignoring.");
      }
    } else {
      // Handle unknown message types if necessary
      Serial.println("Unknown message type received, ignoring.");
    }
  }
}

