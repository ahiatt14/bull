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

#endif