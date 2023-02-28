#ifndef __TAIL_VEC3__
#define __TAIL_VEC3__

#include "precision.h"

typedef struct TAILVEC2 {
  float x, y;
} Vec2;

typedef struct TAILVEC3 {
  float x, y, z;
} Vec3;

Vec2 vec2__normalize(
  Vec2 t
);
float vec2__magnitude(
  Vec2 t
);
float vec2__dot(
  Vec2 t0,
  Vec2 t1
);
float vec2__fewest_rads_btw_vec2s(
  Vec2 t0,
  Vec2 t1
);
float vec2__dist_from_point_to_line(
  Vec2 p0,
  Vec2 p1,
  Vec2 test_position
);

Vec3 vec3_plus_vec3(
  Vec3 t0,
  Vec3 t1
);
Vec3 vec3_minus_vec3(
  Vec3 t0,
  Vec3 t1
);
float vec3__distance(
  Vec3 t0,
  Vec3 t1
);
float vec3__magnitude(
  Vec3 t
);
Vec3 vec3__normalize(
  Vec3 t
);
float vec3__dot(
  Vec3 t0,
  Vec3 t1
);
Vec3 vec3__cross(
  Vec3 t0,
  Vec3 t1
);
Vec3 vec3__mean(
  Vec3 const *const vec3s,
  int count
);
Vec3 vec3__negate(
  Vec3 t
);
Vec3 scalar_x_vec3(
  float s,
  Vec3 t
);

Vec2 scalar_x_vec2(
  float s,
  Vec2 t
);

#endif