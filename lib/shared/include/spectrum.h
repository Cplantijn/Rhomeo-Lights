#ifndef SPECTRUM_H
#define SPECTRUM_H
 
#include <cstdint> // For uint8_t
#include <vector>


struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

#define COLOR_BLAZING_RED       {255, 0, 0}      // Pure Red
#define COLOR_FIERY_RED         {255, 36, 69}    // Fiery Red
#define COLOR_SUNRISE_ORANGE    {255, 69, 0}     // Sunrise Orange
#define COLOR_ROYAL_GOLD        {255, 221, 0}    // Royal Gold
#define COLOR_NEON_YELLOW       {232, 255, 0}    // Neon Yellow
#define COLOR_ELECTRIC_LIME     {204, 255, 0}    // Electric Lime
#define COLOR_CYBER_GREEN       {55, 255, 139}   // Cyber Green
#define COLOR_NEON_TEAL         {0, 255, 208}    // Neon Teal
#define COLOR_AQUA_GLOW         {0, 255, 246}    // Aqua Glow
#define COLOR_ELECTRIC_BLUE     {125, 249, 255}  // Electric Blue
#define COLOR_PLASMA_BLUE       {50, 200, 255}   // Plasma Blue
#define COLOR_TWILIGHT_BLUE     {58, 87, 255}    // Twilight Blue
#define COLOR_NEBULA_VIOLET     {155, 48, 255}   // Nebula Violet
#define COLOR_NEON_PURPLE       {138, 43, 226}   // Neon Purple
#define COLOR_VIBRANT_PURPLE    {179, 58, 243}   // Vibrant Purple
#define COLOR_LASER_PURPLE      {212, 0, 255}    // Laser Purple
#define COLOR_MYSTIC_MAGENTA    {255, 0, 255}    // Mystic Magenta
#define COLOR_COSMIC_PINK       {255, 78, 145}   // Cosmic Pink
#define COLOR_NEON_PINK         {255, 110, 199}  // Neon Pink
#define COLOR_HYPER_PINK        {255, 0, 170}    // Hyper Pink

extern const Color SPECTRUM_COLORS[];
extern const int NUM_SPECTRUM_COLORS;
#define SPECTRUM_STEPS 100 // 5 steps per color (20)

std::vector<Color> generateSpectrumSteps(int steps);
uint16_t colorTo565(const Color& color); 

#endif
