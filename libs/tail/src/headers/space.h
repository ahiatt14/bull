#ifndef __TAIL_SPACE__
#define __TAIL_SPACE__

#include "quaternion.h"
#include "m4x4.h"
#include "vector.h"

struct coordinate_space {
  struct vec3 up;
  struct vec3 right;
  struct vec3 forward;
};

struct transform {
  struct vec3 position;
  struct quaternion _rotation;
  float scale;
};

struct vec3 space__ccw_angle_rotate(
  struct vec3 axis,
  float radians,
  struct vec3 point
);

struct vec3 space__ccw_quat_rotate(
  struct quaternion q,
  struct vec3 point
);

void space__create_model(
  struct coordinate_space const *const space,
  struct transform const *const t,
  struct m4x4 *const dest
);

void space__create_normals_model(
  struct m4x4 const *const model,
  struct m3x3 *const dest
);

#endif