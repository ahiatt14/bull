#ifndef __TAIL_QUATERNION__
#define __TAIL_QUATERNION__

#include "vector.h"
#include "m4x4.h"

struct quaternion {
  struct vec3 v;
  float w;
};

struct quaternion quaternion__multiply(
  struct quaternion lp,
  struct quaternion rp
);

struct quaternion quaternion__create(
  struct vec3 axis,
  float radians
);

void quaternion__to_m4x4(
  struct quaternion p,
  struct m4x4 *const m
);

#endif