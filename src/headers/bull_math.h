#ifndef __BULL_MATH__
#define __BULL_MATH__

#include "tail.h"

float rads_ccw_from_forward_around_up(
  struct vec3 t
);

const struct m4x4* camera__calculate_ortho(
  float r,
  float t,
  float n,
  float f,
  struct camera *const cam
);

float loop_float(float v, float min, float max);

struct vec3 vec3__lerp(
  struct vec3 t0,
  struct vec3 t1,
  float p
);

#endif