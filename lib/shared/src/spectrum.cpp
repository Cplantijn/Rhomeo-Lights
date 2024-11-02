#include <spectrum.h>
#include <cmath>

const Color SPECTRUM_COLORS[] = {
  COLOR_BLAZING_RED,
  COLOR_FIERY_RED,
  COLOR_SUNRISE_ORANGE,
  COLOR_ROYAL_GOLD,
  COLOR_NEON_YELLOW,
  COLOR_ELECTRIC_LIME,
  COLOR_CYBER_GREEN,
  COLOR_NEON_TEAL,
  COLOR_AQUA_GLOW,
  COLOR_ELECTRIC_BLUE,
  COLOR_PLASMA_BLUE,
  COLOR_TWILIGHT_BLUE,
  COLOR_NEBULA_VIOLET,
  COLOR_NEON_PURPLE,
  COLOR_VIBRANT_PURPLE,
  COLOR_LASER_PURPLE,
  COLOR_MYSTIC_MAGENTA,
  COLOR_COSMIC_PINK,
  COLOR_NEON_PINK,
  COLOR_HYPER_PINK
};

const int NUM_SPECTRUM_COLORS = sizeof(SPECTRUM_COLORS) / sizeof(SPECTRUM_COLORS[0]);

uint16_t colorTo565(const Color& color) {
  // Extract the most significant bits for each color component in the 565 format
  // 5 bits for red, 6 bits for green, and 5 bits for blue
  uint16_t r = (color.r >> 3) & 0x1F; // Shift right to discard the least significant 3 bits
  uint16_t g = (color.g >> 2) & 0x3F; // Shift right to discard the least significant 2 bits
  uint16_t b = (color.b >> 3) & 0x1F; // Shift right to discard the least significant 3 bits

  // Combine the bits: red (5 bits) shifted left by 11 bits, 
  // green (6 bits) shifted left by 5 bits, and blue (5 bits) as is
  return (r << 11) | (g << 5) | b;
}