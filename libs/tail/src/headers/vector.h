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
float vec2__fewest_rads_btw_vec2s(
  struct vec2 t0,
  struct vec2 t1
);
float vec2__dist_from_point_to_line(
  struct vec2 p0,
  struct vec2 p1,
  struct vec2 test_position
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