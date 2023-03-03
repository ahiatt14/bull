#ifndef __BULL_CONSTANTS__
#define __BULL_CONSTANTS__

#include "tail.h"

#define DELTA_CAP 1.0f / 30.0f

// TODO: find better place for this
typedef struct BULLCONTROLLERACTIONS {
  void (*on_start_autofire)();
  void (*on_stop_autofire)();
} ControllerActions;

typedef double Seconds;

extern M3x3 M3X3_IDENTITY;
extern M4x4 M4X4_IDENTITY;

extern Shader FLAT_TEXTURE_SHADER;
extern Shader SOLID_COLOR_SHADER;
extern Shader NORMALS_COLOR_SHADER;
extern Shader NORMALS_VIS_SHADER;
extern Shader DEFAULT_BILLBOARD_SHADER;
extern Shader SKYBOX_SHADER;

extern PointBuffer POINT;
extern DrawableMesh QUAD;

extern const Vec3 COLOR_MALACHITE;
extern const Vec3 COLOR_WHITE;
extern const Vec3 COLOR_LIGHT_GREY;
extern const Vec3 COLOR_BLACK;
extern const Vec3 COLOR_RED;
extern const Vec3 COLOR_GREEN;
extern const Vec3 COLOR_BLUE;
extern const Vec3 COLOR_SKY_BLUE;
extern const Vec3 COLOR_GOLDEN_YELLOW;
extern const Vec3 COLOR_EVENING_SUNLIGHT;
extern const Vec3 COLOR_NEON_PURPLE;
extern const Vec3 COLOR_AQUA_BLUE;
extern const Vec3 COLOR_MAGENTA_WHITE;
extern const Vec3 COLOR_DEEP_FOREST_GREEN;
extern const Vec3 COLOR_ROYAL_GREEN;
extern const Vec3 COLOR_DARK_GREY_BLUE;
extern const Vec3 COLOR_DARK_SLATE_GREY;
extern const Vec3 COLOR_BLOOD_RED;
extern const Vec3 COLOR_HOT_PINK;
extern const Vec3 COLOR_OCEAN_BLUE;

extern const Vec3 ORIGIN;
extern const CoordinateSpace WORLDSPACE;

#endif