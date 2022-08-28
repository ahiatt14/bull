#include "tail.h"

#include "constants.h"

struct shader FLAT_TEXTURE_SHADER;
struct shader SOLID_COLOR_SHADER;
struct shader NORMALS_COLOR_SHADER;
struct shader NORMALS_VIS_SHADER;

const struct vec3 COLOR_WHITE = { 1, 1, 1 };
const struct vec3 COLOR_LIGHT_GREY = { 0.86f, 0.86f, 0.86f };
const struct vec3 COLOR_BLACK = { 0, 0, 0 };
const struct vec3 COLOR_RED = { 1, 0, 0 };
const struct vec3 COLOR_SKY_BLUE = {
  135.0f / 255.0f,
  206.0f / 255.0f,
  235.0f / 255.0f
};
const struct vec3 COLOR_GOLDEN_YELLOW = {
  255.0f / 255.0f,
  184.0f / 255.0f,
  28.0f / 255.0f
};
const struct vec3 COLOR_NEON_PURPLE = {
  199.0f / 255.0f,
  36.0f / 255.0f,
  177.0f / 255.0f
};
const struct vec3 COLOR_EVENING_SUNLIGHT = {
  234.0 / 255.0,
  208.0 / 255.0,
  146.0 / 255.0
};
const struct vec3 COLOR_AQUA_BLUE = {
  0.4f,
  0.56f,
  0.63f
};
const struct vec3 COLOR_MAGENTA_WHITE = {
  .95f,
  .9f,
  .93f
};

const struct vec3 ORIGIN = { 0, 0, 0 };
const struct coordinate_space WORLDSPACE = {
  .up = { 0, 1, 0 },
  .right = { 1, 0, 0 },
  .forward = { 0, 0, 1 },
};