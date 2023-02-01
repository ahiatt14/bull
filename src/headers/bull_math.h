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

double dmax(double v, double max);

float floop(float v, float min, float max);

struct Vec3 vec3__linear_lerp(
  struct Vec3 t0,
  struct Vec3 t1,
  double ratio
);

struct Vec3 vec3__square_lerp(
  struct Vec3 t0,
  struct Vec3 t1,
  double ratio
);

#endif