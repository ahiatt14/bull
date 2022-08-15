#ifndef __TAIL_SPACE__
#define __TAIL_SPACE__

#include "m4x4.h"
#include "vector.h"

struct coordinate_space {
  struct vec3 up;
  struct vec3 right;
  struct vec3 forward;
};

struct transform {
  struct vec3 position;
  struct vec3 rotation_in_deg;
  float scale;
};

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