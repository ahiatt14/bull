#ifndef __TAIL_VEC3__
#define __TAIL_VEC3__

#include "precision.h"

struct vec2 {
  float x, y;
};

struct vec3 {
  float x, y, z;
};

void vec3__create(float x, float y, float z, struct vec3 *const t);
void vec3_plus_vec3(
  struct vec3 const *const t0,
  struct vec3 const *const t1,
  struct vec3 *const sum
);
void vec3_minus_vec3(
  struct vec3 const *const t0,
  struct vec3 const *const t1,
  struct vec3 *const diff
);
float vec3__magnitude(struct vec3 const *const t);
float vec2__magnitude(struct vec2 const *const t);
void vec3__normalize(struct vec3 const *const t, struct vec3 *const normalized);
void vec2__normalize(struct vec2 const *const t, struct vec2 *const normalized);
void vec3__mean(
  struct vec3 const *const vec3s,
  int count,
  struct vec3 *const avg
);
float vec3__dot(
  struct vec3 const *const t0,
  struct vec3 const *const t1
);
void vec3__cross(
  struct vec3 const *const t0,
  struct vec3 const *const t1,
  struct vec3 *const cross
);

#endif