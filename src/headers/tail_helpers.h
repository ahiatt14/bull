#ifndef __BULL_GPU_HELPERS__
#define __BULL_GPU_HELPERS__

#include "tail.h"

typedef struct BULLGAMETIME {
  double delta;
  double sec_since_game_launch;
} GameTime;

void gpu__set_mvp(
  M4x4 const *const model,
  M3x3 const *const normals_model,
  Camera const *const cam,
  Shader const *const shad,
  GPU const *const gpu
);

void mesh__tile_uvs(
  float x_multiplier,
  float y_multiplier,
  DrawableMesh *const mesh
);

// TODO: add space__euler_to_basis fn

#endif