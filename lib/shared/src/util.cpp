#include <cmath>

int normalizeRangeInt(int inputValue, int minInput, int maxInput, int minOutput, int maxOutput) {
  double inputRange = maxInput - minInput;
  double outputRange = maxOutput - minOutput;
  double normalizedValue = (inputValue - minInput) / inputRange;
  
  return int(round(normalizedValue * outputRange) + minOutput);
}