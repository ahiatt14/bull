#include "bull_math.h"

char array_contains_u_i(
  unsigned int value,
  unsigned int *values,
  int count
) {
  for (int i = 0; i < count; i++) if (values[i] == value) return 1;
  return 0;
}

struct vec2 vec2__turn_90_deg(
  uint8_t left,
  struct vec2 src
) {
  return left ?
    ((struct vec2){ -src.y, src.x }) : 
    ((struct vec2){ src.y, -src.x });
}