#ifndef __TAIL_SPACE__
#define __TAIL_SPACE__

#include "quaternion.h"
#include "m4x4.h"
#include "vector.h"

struct CoordinateSpace {
  struct Vec3 up;
  struct Vec3 right;
  struct Vec3 forward;
};

struct Transform {
  struct Vec3 position;
  struct Quaternion rotation;
  float scale;
};

struct Vec3 space__ccw_angle_rotate(
  struct Vec3 axis,
  float radians,
  struct Vec3 point
);

struct Vec3 space__ccw_quat_rotate(
  struct Quaternion q,
  struct Vec3 point
);

void space__create_model(
  struct CoordinateSpace const *const space,
  struct Transform const *const t,
  struct M4x4 *const dest
);

void space__create_normals_model(
  struct M4x4 const *const model,
  struct M3x3 *const dest
);

#endif