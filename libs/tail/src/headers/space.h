#ifndef __TAIL_SPACE__
#define __TAIL_SPACE__

#include "quaternion.h"
#include "m4x4.h"
#include "vector.h"

typedef struct TAILCOORDINATESPACE {
  Vec3 up;
  Vec3 right;
  Vec3 forward;
} CoordinateSpace;

typedef struct TAILTRANSFORM {
  Vec3 position;
  Quaternion rotation;
  float scale;
} Transform;

Vec3 space__ccw_angle_rotate(
  Vec3 axis,
  float radians,
  Vec3 point
);

Vec3 space__ccw_quat_rotate(
  Quaternion q,
  Vec3 point
);

void space__create_model(
  CoordinateSpace const *const space,
  Transform const *const t,
  M4x4 *const dest
);

void space__create_normals_model(
  M4x4 const *const model,
  M3x3 *const dest
);

#endif