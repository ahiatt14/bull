#ifndef __TAIL_VEC3__
#define __TAIL_VEC3__

#include "precision.h"

struct Vec2 {
  float x, y;
};

struct Vec2 vec2__normalize(
  struct Vec2 t
);
float vec2__magnitude(
  struct Vec2 t
);
float vec2__dot(
  struct Vec2 t0,
  struct Vec2 t1
);
float vec2__fewest_rads_btw_vec2s(
  struct Vec2 t0,
  struct Vec2 t1
);
float vec2__dist_from_point_to_line(
  struct Vec2 p0,
  struct Vec2 p1,
  struct Vec2 test_position
);

struct Vec3 {
  float x, y, z;
};

struct Vec3 vec3_plus_vec3(
  struct Vec3 t0,
  struct Vec3 t1
);
struct Vec3 vec3_minus_vec3(
  struct Vec3 t0,
  struct Vec3 t1
);
float vec3__distance(
  struct Vec3 t0,
  struct Vec3 t1
);
float vec3__magnitude(
  struct Vec3 t
);
struct Vec3 vec3__normalize(
  struct Vec3 t
);
float vec3__dot(
  struct Vec3 t0,
  struct Vec3 t1
);
struct Vec3 vec3__cross(
  struct Vec3 t0,
  struct Vec3 t1
);
struct Vec3 vec3__mean(
  struct Vec3 const *const vec3s,
  int count
);
struct Vec3 vec3__negate(
  struct Vec3 t
);
struct Vec3 scalar_x_vec3(
  float s,
  struct Vec3 t
);

#endif