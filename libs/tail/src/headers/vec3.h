#ifndef __TAIL_VEC3__
#define __TAIL_VEC3__

#include "precision.h"

struct vec3 {
  float x, y, z;
};

void vec3_create(float x, float y, float z, struct vec3 *t);
void vec3_plus_vec3(
  const struct vec3 *t0,
  const struct vec3 *t1,
  struct vec3 *sum
);
void vec3_minus_vec3(
  const struct vec3 *t0,
  const struct vec3 *t1,
  struct vec3 *diff
);
float vec3_magnitude(const struct vec3 *t);
void vec3_normalize(const struct vec3 *t, struct vec3 *normalized);
void vec3_mean(const struct vec3 *vec3s, int count, struct vec3 *avg);
float vec3_dot(const struct vec3 *t0, const struct vec3 *t1);
void vec3_cross(
  const struct vec3 *t0,
  const struct vec3 *t1,
  struct vec3 *cross
);
int vec3_equals_vec3(
  const struct vec3 *t0,
  const struct vec3 *t1,
  const struct float_tolerance *ft // TODO: this is dumb, just pass the tolerance value
);

#endif