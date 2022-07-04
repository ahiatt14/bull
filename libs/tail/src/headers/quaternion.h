#ifndef __TAIL_QUATERNION__
#define __TAIL_QUATERNION__

#include "vec3.h"
#include "m4x4.h"

struct quaternion {
  struct vec3 v;
  float w;
};

void quaternion__multiply(
  struct quaternion const *const left_p,
  struct quaternion const *const right_p,
  struct quaternion *const dest
);
void quaternion__conjugate(
  struct quaternion const *const p,
  struct quaternion *const dest
);
void quaternion__create(
  struct vec3 const *const axis,
  float radians,
  struct quaternion *const dest
);
void quaternion__to_m4x4(
  struct quaternion const *const p,
  struct m4x4 *const m
);

#endif