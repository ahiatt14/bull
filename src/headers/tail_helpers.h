#ifndef __BULL_GPU_HELPERS__
#define __BULL_GPU_HELPERS__

#include "tail.h"

struct GameTime {
  double delta;
  double sec_since_game_launch;
};

void gpu__set_mvp(
  struct M4x4 const *const model,
  struct M3x3 const *const normals_model,
  struct Camera const *const cam,
  struct Shader const *const shad,
  struct GPU const *const gpu
);

void mesh__tile_uvs(
  float x_multiplier,
  float y_multiplier,
  struct DrawableMesh *const mesh
);

// TODO: add space__euler_to_basis fn

#endif