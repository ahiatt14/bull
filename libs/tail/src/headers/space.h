#ifndef __TAIL_SPACE__
#define __TAIL_SPACE__

#include "m4x4.h"
#include "vec3.h"

struct coordinate_space {
  struct vec3 up;
  struct vec3 right;
  struct vec3 forward;
};

struct transform {
  struct vec3 position;
  struct vec3 rotation_in_deg; // TODO: we'll want to represent rotation as quats eventually
  float scale;
};

void space__create_model(
  const struct coordinate_space *space,
  const struct transform *t,
  struct m4x4 *model
);

#endif