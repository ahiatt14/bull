#ifndef __TAIL_VEC3__
#define __TAIL_VEC3__

#include "precision.h"

struct vec3 {
  float x, y, z;
};

void vec3__create(float x, float y, float z, struct vec3 *t);
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
float vec3__magnitude(const struct vec3 *t);
void vec3__normalize(const struct vec3 *t, struct vec3 *normalized);
void vec3__mean(const struct vec3 *vec3s, int count, struct vec3 *avg);
float vec3__dot(const struct vec3 *t0, const struct vec3 *t1);
void vec3__cross(
  const struct vec3 *t0,
  const struct vec3 *t1,
  struct vec3 *cross
);

#endif