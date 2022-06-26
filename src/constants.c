#include "tail.h"

#include "constants.h"

const struct vec3 COLOR_WHITE = { 1, 1, 1 };
const struct vec3 COLOR_LIGHT_GREY = { 0.86f, 0.86f, 0.86f };
const struct vec3 COLOR_BLACK = { 0, 0, 0 };
const struct vec3 COLOR_RED = { 1, 0, 0 };

const struct vec3 ORIGIN = { 0, 0, 0 };
const struct coordinate_space WORLDSPACE = {
  .up = { 0, 1, 0 },
  .right = { 1, 0, 0 },
  .forward = { 0, 0, 1 },
};