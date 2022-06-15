#ifndef __BULL_GPU_HELPERS__
#define __BULL_GPU_HELPERS__

#include "tail.h"

void gpu__set_mvp(
  const struct m4x4 *model,
  const struct m4x4 *view,
  const struct m4x4 *perspective,
  const struct m3x3 *normals_model,
  const struct gpu_program *prog,
  const struct gpu_api *gpu
);

#endif