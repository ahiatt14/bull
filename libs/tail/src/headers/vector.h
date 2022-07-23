#ifndef __TAIL_VEC3__
#define __TAIL_VEC3__

#include "precision.h"

struct vec2 {
  float x, y;
};

struct vec2 vec2__normalize(
  struct vec2 t
);
float vec2__magnitude(
  struct vec2 t
);
float vec2__dot(
  struct vec2 t0,
  struct vec2 t1
);

struct vec3 {
  float x, y, z;
};

struct vec3 vec3_plus_vec3(
  struct vec3 t0,
  struct vec3 t1
);
struct vec3 vec3_minus_vec3(
  struct vec3 t0,
  struct vec3 t1
);
float vec3__distance(
  struct vec3 t0,
  struct vec3 t1
);
float vec3__magnitude(
  struct vec3 t
);
struct vec3 vec3__normalize(
  struct vec3 t
);
float vec3__dot(
  struct vec3 t0,
  struct vec3 t1
);
struct vec3 vec3__cross(
  struct vec3 t0,
  struct vec3 t1
);
struct vec3 vec3__mean(
  struct vec3 const *const vec3s,
  int count
);
struct vec3 vec3__negate(
  struct vec3 t
);
struct vec3 scalar_x_vec3(
  float s,
  struct vec3 t
);

#endif