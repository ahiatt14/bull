#ifndef __BULL_MATH__
#define __BULL_MATH__

#include "tail.h"

float rads_ccw_from_forward_around_up(
  struct Vec3 t
);

const struct M4x4* camera__calculate_ortho(
  float r,
  float t,
  float n,
  float f,
  struct Camera *const cam
);

float loop_float(float v, float min, float max);

struct Vec3 vec3__lerp(
  struct Vec3 t0,
  struct Vec3 t1,
  float p
);

#endif