#ifndef __BULL_GPU_HELPERS__
#define __BULL_GPU_HELPERS__

#include "tail.h"

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

struct vec3 space__transform_space(
  struct coordinate_space const *const space,
  struct transform const *const trans
);

struct vec3 space__world_to_ndc(
  struct camera const *const cam,
  struct vec3 pos
);

#endif