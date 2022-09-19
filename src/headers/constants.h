#ifndef __BULL_CONSTANTS__
#define __BULL_CONSTANTS__

#include "tail.h"

#define BULL_MAX_TEXTURE_COUNT 100 // TODO: ?
#define DELTA_TIME_CAP 1.0f / 30.0f

extern struct m3x3 M3X3_IDENTITY;
extern struct m4x4 M4X4_IDENTITY;

extern struct shader FLAT_TEXTURE_SHADER;
extern struct shader SOLID_COLOR_SHADER;
extern struct shader NORMALS_COLOR_SHADER;
extern struct shader NORMALS_VIS_SHADER;

extern struct drawable_mesh QUAD;

extern const struct vec3 COLOR_WHITE;
extern const struct vec3 COLOR_LIGHT_GREY;
extern const struct vec3 COLOR_BLACK;
extern const struct vec3 COLOR_RED;
extern const struct vec3 COLOR_GREEN;
extern const struct vec3 COLOR_BLUE;
extern const struct vec3 COLOR_SKY_BLUE;
extern const struct vec3 COLOR_GOLDEN_YELLOW;
extern const struct vec3 COLOR_EVENING_SUNLIGHT;
extern const struct vec3 COLOR_NEON_PURPLE;
extern const struct vec3 COLOR_AQUA_BLUE;
extern const struct vec3 COLOR_MAGENTA_WHITE;
extern const struct vec3 COLOR_DEEP_FOREST_GREEN;
extern const struct vec3 COLOR_DARK_GREY_BLUE;
extern const struct vec3 COLOR_DARK_SLATE_GREY;
extern const struct vec3 COLOR_BLOOD_RED;
extern const struct vec3 COLOR_HOT_PINK;

extern const struct vec3 ORIGIN;
extern const struct coordinate_space WORLDSPACE;

#endif