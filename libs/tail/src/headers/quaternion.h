#ifndef __TAIL_QUATERNION__
#define __TAIL_QUATERNION__

#include "vector.h"
#include "m4x4.h"

typedef struct TAILQUATERNION {
  Vec3 v;
  float w;
} Quaternion;

Quaternion quaternion__multiply(
  Quaternion lp,
  Quaternion rp
);

Quaternion quaternion__create(
  Vec3 axis,
  float radians
);

void quaternion__to_m4x4(
  Quaternion p,
  M4x4 *const m
);

Quaternion quaternion__linear_slerp(
  Quaternion q0,
  Quaternion q1,
  float t
);

#endif