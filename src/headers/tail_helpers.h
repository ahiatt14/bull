#ifndef __BULL_GPU_HELPERS__
#define __BULL_GPU_HELPERS__

#include "tail.h"

struct gametime {
  double delta;
  double sec_since_game_launch;
};

void gpu__set_mvp(
  struct m4x4 const *const model,
  struct m3x3 const *const normals_model,
  struct camera const *const cam,
  struct shader const *const shad,
  struct gpu_api const *const gpu
);

void mesh__tile_uvs(
  float x_multiplier,
  float y_multiplier,
  struct drawable_mesh *const mesh
);

// TODO: add space__euler_to_basis fn

#endif