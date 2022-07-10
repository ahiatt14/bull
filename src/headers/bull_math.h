#ifndef __BULL_MATH__
#define __BULL_MATH__

#include <stdint.h>

#include "tail.h"

char array_contains_u_i(
  unsigned int value,
  unsigned int *values,
  int count
);

struct vec2 vec2__turn_90_deg(
  uint8_t left,
  struct vec2 src
);

#endif