#ifndef __BULL_CONSTANTS__
#define __BULL_CONSTANTS__

#include "tail.h"

#define DELTA_CAP 1.0f / 30.0f

#define ARENA_RADIUS 8.0f

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

extern const Vec3 ORIGIN;
extern const CoordinateSpace WORLDSPACE;

#endif