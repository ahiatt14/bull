#ifndef __TAIL_QUATERNION__
#define __TAIL_QUATERNION__

#include "vec3.h"
#include "m4x4.h"

struct quaternion {
  struct vec3 v;
  float w;
};

void quaternion__multiply(
  const struct quaternion *left_p,
  const struct quaternion *right_p,
  struct quaternion *dest
);
void quaternion__conjugate(
  const struct quaternion *p,
  struct quaternion *dest
);
void quaternion__create(
  const struct vec3 *axis,
  float radians,
  struct quaternion *dest
);
void quaternion__to_m4x4(
  const struct quaternion *p,
  struct m4x4 *m
);

#endif