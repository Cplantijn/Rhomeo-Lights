#include <spectrum.h>
#include <cmath>

Color generateSpectrumColor(double position) {
  Color color;

  // Cycle position within 0 to 1 range
  position = fmod(position, 1.0);

  // Calculate color components based on position
  if (position < 1.0 / 6.0) {
    color.r = 255;
    color.g = static_cast<uint8_t>(255 * position * 6);
    color.b = 0;
  } else if (position < 2.0 / 6.0) {
    color.r = static_cast<uint8_t>(255 * (2 - position * 6));
    color.g = 255;
    color.b = 0;
  } else if (position < 3.0 / 6.0) {
    color.r = 0;
    color.g = 255;
    color.b = static_cast<uint8_t>(255 * position * 6 - 2); 
  } else if (position < 4.0 / 6.0) {
    color.r = 0;
    color.g = static_cast<uint8_t>(255 * (4 - position * 6));
    color.b = 255;
  } else if (position < 5.0 / 6.0) {
    color.r = static_cast<uint8_t>(255 * position * 6 - 4);
    color.g = 0;
    color.b = 255;
  } else {
    color.r = 255;
    color.g = 0;
    color.b = static_cast<uint8_t>(255 * (6 - position * 6));
  }

  return color;
}

std::vector<Color> generateSpectrumSteps(int steps) {
  std::vector<Color> spectrum;
  spectrum.reserve(steps); // Pre-allocate space for efficiency

  double stepSize = 1.0 / steps;
  for (double position = 0.0; position < 1.0; position += stepSize) {
    spectrum.push_back(generateSpectrumColor(position));
  }

  return spectrum;
}


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