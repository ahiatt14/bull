#ifndef __BULL_CONSTANTS__
#define __BULL_CONSTANTS__

#include "tail.h"

#define DELTA_CAP 1.0f / 30.0f

extern struct M3x3 M3X3_IDENTITY;
extern struct M4x4 M4X4_IDENTITY;

extern struct Shader FLAT_TEXTURE_SHADER;
extern struct Shader SOLID_COLOR_SHADER;
extern struct Shader NORMALS_COLOR_SHADER;
extern struct Shader NORMALS_VIS_SHADER;

extern struct PointBuffer POINT;
extern struct DrawableMesh QUAD;

extern const struct Vec3 COLOR_MALACHITE;
extern const struct Vec3 COLOR_WHITE;
extern const struct Vec3 COLOR_LIGHT_GREY;
extern const struct Vec3 COLOR_BLACK;
extern const struct Vec3 COLOR_RED;
extern const struct Vec3 COLOR_GREEN;
extern const struct Vec3 COLOR_BLUE;
extern const struct Vec3 COLOR_SKY_BLUE;
extern const struct Vec3 COLOR_GOLDEN_YELLOW;
extern const struct Vec3 COLOR_EVENING_SUNLIGHT;
extern const struct Vec3 COLOR_NEON_PURPLE;
extern const struct Vec3 COLOR_AQUA_BLUE;
extern const struct Vec3 COLOR_MAGENTA_WHITE;
extern const struct Vec3 COLOR_DEEP_FOREST_GREEN;
extern const struct Vec3 COLOR_ROYAL_GREEN;
extern const struct Vec3 COLOR_DARK_GREY_BLUE;
extern const struct Vec3 COLOR_DARK_SLATE_GREY;
extern const struct Vec3 COLOR_BLOOD_RED;
extern const struct Vec3 COLOR_HOT_PINK;

extern const struct Vec3 ORIGIN;
extern const struct CoordinateSpace WORLDSPACE;

#endif