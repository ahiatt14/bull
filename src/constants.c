#include "tail.h"

#include "constants.h"

struct M3x3 M3X3_IDENTITY = (struct M3x3){{
  1, 0, 0,
  0, 1, 0,
  0, 0, 1
}};
struct M4x4 M4X4_IDENTITY = (struct M4x4){{
  1, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1, 0,
  0, 0, 0, 1
}};

struct Shader FLAT_TEXTURE_SHADER;
struct Shader SOLID_COLOR_SHADER;
struct Shader NORMALS_COLOR_SHADER;
struct Shader NORMALS_VIS_SHADER;

struct PointBuffer POINT = (struct PointBuffer){
  .points = (struct Vec3[1]){{ 0, 0, 0 }},
  .points_length = 1
};

struct DrawableMesh QUAD = (struct DrawableMesh){
  .vertices = (struct Vertex[4]){
    {{ -0.5f, -0.5f, 0 }, { 0, 1, 0 }, { 0, 0 }},
    {{ 0.5f, -0.5f, 0 }, { 0, 1, 0 }, { 1, 0 }},
    {{ 0.5f, 0.5f, 0 }, { 0, 1, 0 }, { 1, 1 }},
    {{ -0.5f, 0.5f, 0 }, { 0, 1, 0 }, { 0, 1 }}
  },
  .indices = (unsigned int[6]){
    0, 1, 2,
    0, 2, 3
  },
  .vertices_size = sizeof(struct Vertex) * 4,
  .indices_size = sizeof(unsigned int) * 6,
  .indices_length = 6
};

const struct Vec3 COLOR_WHITE = { 1, 1, 1 };
const struct Vec3 COLOR_LIGHT_GREY = { 0.86f, 0.86f, 0.86f };
const struct Vec3 COLOR_BLACK = { 0, 0, 0 };

const struct Vec3 COLOR_RED = { 1, 0, 0 };
const struct Vec3 COLOR_GREEN = { 0, 1, 0 };
const struct Vec3 COLOR_BLUE = { 0, 0, 1 };

const struct Vec3 COLOR_MALACHITE = {
  0.20f,
  1.00f,
  0.33f
};
const struct Vec3 COLOR_SKY_BLUE = {
  135.0f / 255.0f,
  206.0f / 255.0f,
  235.0f / 255.0f
};
const struct Vec3 COLOR_GOLDEN_YELLOW = {
  255.0f / 255.0f,
  184.0f / 255.0f,
  28.0f / 255.0f
};
const struct Vec3 COLOR_NEON_PURPLE = {
  199.0f / 255.0f,
  36.0f / 255.0f,
  177.0f / 255.0f
};
const struct Vec3 COLOR_EVENING_SUNLIGHT = {
  234.0 / 255.0,
  208.0 / 255.0,
  146.0 / 255.0
};
const struct Vec3 COLOR_AQUA_BLUE = {
  0.4f,
  0.56f,
  0.63f
};
const struct Vec3 COLOR_MAGENTA_WHITE = {
  .95f,
  .9f, 
  .93f
};
const struct Vec3 COLOR_DEEP_FOREST_GREEN = {
  31.0f / 255.0f,
  61.0f / 255.0f,
  12.0f / 255.0f
};
const struct Vec3 COLOR_ROYAL_GREEN = {
  0.0745f,
  0.3843f,
  0.0275f
};
const struct Vec3 COLOR_DARK_GREY_BLUE = {
  66.0f / 255.0f,
  73.0f / 255.0f,
  82.0f / 255.0f
};
const struct Vec3 COLOR_DARK_SLATE_GREY = {
  61.0f / 255.0f,
  72.0f / 255.0f,
  73.0f / 255.0f
};

const struct Vec3 COLOR_BLOOD_RED = {
  166.0f / 255.0f,
  16.0f / 255.0f,
  30.0f / 255.0f
};

const struct Vec3 COLOR_HOT_PINK = {
  255.0f / 255.0f,
  56.0f / 255.0f,
  146.0f / 255.0f
};

const struct Vec3 ORIGIN = { 0, 0, 0 };
const struct CoordinateSpace WORLDSPACE = {
  .up = { 0, 1, 0 },
  .right = { 1, 0, 0 },
  .forward = { 0, 0, -1 },
};