#ifndef __TAIL_MATH__
#define __TAIL_MATH__

#include "precision.h"

float deg_to_rad(float d);
int vec_equals_vec(
  const float *x0,
  const float *x1,
  unsigned short int element_count,
  const struct float_tolerance *ft
);
int iclamp(int v, int min, int max);

#endif