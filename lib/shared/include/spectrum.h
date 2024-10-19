#ifndef SPECTRUM_H
#define SPECTRUM_H
 
#include <cstdint> // For uint8_t
#include <vector>

#define SPECTRUM_STEPS 128

struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

std::vector<Color> generateSpectrumSteps(int steps);
uint16_t colorTo565(const Color& color); 

#endif
