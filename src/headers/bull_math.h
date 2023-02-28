#ifndef __BULL_MATH__
#define __BULL_MATH__

#include "tail.h"

float rads_ccw_from_forward_around_up(
  Vec3 t
);

const M4x4* camera__calculate_ortho(
  float r,
  float t,
  float n,
  float f,
  Camera *const cam
);

double d_max(double v, double max);
float f_max(float v, float max);

float floop(float v, float min, float max);

Vec3 vec3__linear_lerp(
  Vec3 t0,
  Vec3 t1,
  double ratio
);

Vec3 vec3__square_lerp(
  Vec3 t0,
  Vec3 t1,
  double ratio
);

#endif