#ifndef __BULL_CONSTANTS__
#define __BULL_CONSTANTS__

#include "tail.h"

const float COLOR_LIGHT_GREY[3] = { 0.86f, 0.86f, 0.86f };

const struct vec3 ORIGIN = { 0, 0, 0 };
const struct coordinate_space WORLDSPACE = {
  .up = { 0, 1, 0 },
  .right = { 1, 0, 0 },
  .forward = { 0, 0, -1 },
};

#endif