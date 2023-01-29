#ifndef __TAIL_QUATERNION__
#define __TAIL_QUATERNION__

#include "vector.h"
#include "m4x4.h"

struct Quaternion {
  struct Vec3 v;
  float w;
};

struct Quaternion quaternion__multiply(
  struct Quaternion lp,
  struct Quaternion rp
);

struct Quaternion quaternion__create(
  struct Vec3 axis,
  float radians
);

void quaternion__to_m4x4(
  struct Quaternion p,
  struct M4x4 *const m
);

#endif