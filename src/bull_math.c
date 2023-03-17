#include <math.h>

#include "bull_math.h"
#include "constants.h"

const M4x4* camera__calculate_ortho(
  float r,
  float t,
  float n,
  float f,
  Camera *const cam
) {
  m4x4__create(
    1 / r, 0, 0, 0,
    0, 1 / t, 0, 0,
    0, 0, -2.0f / (f - n), -(f + n) / (f - n),
    0, 0, 0, 1,
    &cam->projection
  );

  return &cam->projection;
}

float rads_ccw_from_forward_around_up(
  Vec3 t
) {
  float rads = atan(-t.x / -t.z);
  if (t.x < 0 && t.z >= 0) {
    rads += M_PI;
  } else if (t.x >= 0 && t.z >= 0) {
    rads += M_PI;
  } else if (t.x >= 0 && t.z < 0) {
    rads += 2 * M_PI;
  }
  return rads;
}

double d_max(double v, double max) {
  return v > max ? max : v;
}

float f_max(float v, float max) {
  return v > max ? max : v;
}

float floop(float v, float min, float max) {
  if (v < min) return max - min - v;
  if (v > max) return min + v - max;
  return v;
}

float flerp(
  float f0,
  float f1,
  float t
) {
  return f0 + (f1 - f0) * t;
}

Vec3 vec3__linear_lerp(
  Vec3 t0,
  Vec3 t1,
  double ratio
) {
  return (Vec3){
    t0.x + ((t1.x - t0.x) * ratio),
    t0.y + ((t1.y - t0.y) * ratio),
    t0.z + ((t1.z - t0.z) * ratio)
  };
}

Vec3 vec3__square_lerp(
  Vec3 t0,
  Vec3 t1,
  double ratio
) {
  return (Vec3){
    t0.x + ((t1.x - t0.x) * ratio * ratio),
    t0.y + ((t1.y - t0.y) * ratio * ratio),
    t0.z + ((t1.z - t0.z) * ratio * ratio)
  };
}