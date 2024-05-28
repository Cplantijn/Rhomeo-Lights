#include "display.h"

Adafruit_GC9A01A tft = Adafruit_GC9A01A(LCD_CS, LCD_DC, LCD_RST);
GFXcanvas16 _offscreenTopHalf = GFXcanvas16(SCREEN_WIDTH, SCREEN_HEIGHT/2);
GFXcanvas16 offscreenPercentage = GFXcanvas16(SCREEN_WIDTH/1.45, SCREEN_HEIGHT/4);

bool isIdleTimeoutActive = false; 
bool isDisplaysIdle = false;
unsigned long idleTimeoutStartTime;

int _sizePerChar = 18;

void fillArc(int start_angle, int seg_count, int w, unsigned int colour) {
  _offscreenTopHalf.fillScreen(0);
  
  int screenWidth = _offscreenTopHalf.width();
  int screenHeight = _offscreenTopHalf.height();
  int x = screenWidth / 2;
  int y = screenHeight;
  int rx = (screenWidth / 2) + 1; // Fills in round error pixels on edges
  int ry = rx;

  byte seg = 2; // Segments are 2 degrees wide = 180 segments for 360 degrees
  byte inc = 2; // Draw segments every 2 degrees, increase to 6 for segmented ring

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

    _offscreenTopHalf.fillTriangle(x0, y0, x1, y1, x2, y2, colour);
    _offscreenTopHalf.fillTriangle(x1, y1, x2, y2, x3, y3, colour);
  }

  tft.drawRGBBitmap(0, 0, _offscreenTopHalf.getBuffer(), screenWidth, screenHeight);
}

void writePercentage(int x, int textColor) {
  // tft.fillRect((SCREEN_WIDTH/6 - 2), SCREEN_HEIGHT/3, SCREEN_WIDTH/1.45, SCREEN_HEIGHT/4, GC9A01A_BLACK);
  offscreenPercentage.fillScreen(0);

  int extraPadLeft = 0;
  if (x < 10) {
    extraPadLeft = _sizePerChar * 2;
  } else if (x < 100) {
    extraPadLeft = _sizePerChar;
  }
  
  int offscreenHeight = offscreenPercentage.height();
  int offScreenWidth = offscreenPercentage.width();

  offscreenPercentage.setFont(&FreeMonoBold18pt7b);
  offscreenPercentage.setCursor(2 + extraPadLeft, (offscreenPercentage.height() / 2) + 16);
  offscreenPercentage.setTextColor(textColor);
  offscreenPercentage.setTextSize(2);
  offscreenPercentage.println(String(x) + "%");
  tft.drawRGBBitmap((SCREEN_WIDTH/6 - 2), SCREEN_HEIGHT/3, offscreenPercentage.getBuffer(), offScreenWidth, offscreenHeight);
}

void writeSelector(char* text) {
  int screenCordLength = int(((SCREEN_WIDTH / 2) * sqrt(63)) / 4);
  int left = (SCREEN_WIDTH - screenCordLength) / 2;
  int top = 160;
  tft.fillRect(left + 20, top, screenCordLength - 40, 32, GC9A01A_BLACK);
  // We have space for 9 chars in one line. Center the characters based on the length of the string
  int textLen = strlen(text);
  int padLeft = (9 - textLen) * (_sizePerChar / 2);
  tft.setCursor(left + 28 + padLeft, top + _sizePerChar + 4);
  tft.setTextColor(GC9A01A_WHITE);
  tft.setTextSize(1);
  tft.println(text);
}

void setDisplaysToActive() {
  digitalWrite(LCD_BL, HIGH);
  tft.sendCommand(GC9A01A_DISPON);
  isDisplaysIdle = false;
}

void setDisplaysToIdle() {
  isDisplaysIdle = true;
  tft.sendCommand(GC9A01A_DISPOFF);
  digitalWrite(LCD_BL, LOW);
}