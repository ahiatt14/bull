#ifndef __TAIL_MATH__
#define __TAIL_MATH__

#include "precision.h"

float deg_to_rad(float d);
float rad_to_deg(float r);
int vec_equals_vec(
  float const *const x0,
  float const *const x1,
  unsigned short int element_count,
  struct float_tolerance const *const ft
);
int iclamp(int v, int min, int max);

#endif