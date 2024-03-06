#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_GC9A01A.h>
#include <Fonts/FreeMonoBold18pt7b.h>

// Screen definitions
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240
#define DEG2RAD 0.0174532925
#define IDLE_TIMEOUT_MS 5000

// Display pins
#define LCD_CS  21
#define LCD_DC  16
#define LCD_RST 4
#define LCD_BL  22

#include <Adafruit_GC9A01A.h>

extern Adafruit_GC9A01A tft;
extern GFXcanvas16 offscreenPercentage;
extern bool isIdleTimeoutActive;
extern bool isDisplaysIdle;
extern unsigned long idleTimeoutStartTime;

void writePercentage(int x, int textColor);
void writeSelector(char* text);
void fillArc(int start_angle, int seg_count, int thickness, unsigned int color);
void setDisplaysToActive();
void setDisplaysToIdle();

#endif