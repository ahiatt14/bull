#include "bull_math.h"

char array_contains_u_i(
  unsigned int value,
  unsigned int *values,
  int count
) {
  for (int i = 0; i < count; i++) if (values[i] == value) return 1;
  return 0;
}