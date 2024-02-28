#include "display.h"

Adafruit_GC9A01A tft = Adafruit_GC9A01A(LCD_CS, LCD_DC);

void fillArc(int x, int y, int start_angle, int seg_count, int rx, int ry, int w, unsigned int colour) {
  byte seg = 3; // Segments are 3 degrees wide = 120 segments for 360 degrees
  byte inc = 3; // Draw segments every 3 degrees, increase to 6 for segmented ring

  // Draw colour blocks every inc degrees
  for (int i = start_angle; i < start_angle + seg * seg_count; i += inc) {
    // Serial.println("i: " + String(i));
    // Calculate pair of coordinates for segment start
    float sx = cos((i - 90) * DEG2RAD);
    float sy = sin((i - 90) * DEG2RAD);
    uint16_t x0 = sx * (rx - w) + x;
    uint16_t y0 = sy * (ry - w) + y;
    uint16_t x1 = sx * rx + x;
    uint16_t y1 = sy * ry + y;

    // Calculate pair of coordinates for segment end
    float sx2 = cos((i + seg - 90) * DEG2RAD);
    float sy2 = sin((i + seg - 90) * DEG2RAD);
    int x2 = sx2 * (rx - w) + x;
    int y2 = sy2 * (ry - w) + y;
    int x3 = sx2 * rx + x;
    int y3 = sy2 * ry + y;

    tft.fillTriangle(x0, y0, x1, y1, x2, y2, colour);
    tft.fillTriangle(x1, y1, x2, y2, x3, y3, colour);
  }
}

void writeText() {
  // tft.fillScreen(GC9A01A_BLACK);
  // tft.fillCircle(120, 120, 122, GC9A01A_RED);
  // tft.fillCircle(120, 120, 94, GC9A01A_BLACK);
  // tft.fillRect(0, 120, 240, 120, GC9A01A_BLACK);
  // tft.setCursor(0, 140);
  // tft.setTextColor(GC9A01A_WHITE);
  tft.setTextSize(1);
  tft.println("Kitchen");
}

void wakeDisplay(uint8_t displayNumber) {
  digitalWrite(LCD_BL, HIGH);
}

void sleepDisplay(uint8_t displayNumber) {
  tft.sendCommand(GC9A01A_DISPOFF);
  digitalWrite(LCD_BL, LOW);
}