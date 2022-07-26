#ifndef __BULL_SQUARE__
#define __BULL_SQUARE__

#include "tail.h"

struct quad {
  struct transform transform;
  struct drawable_mesh mesh;
  struct texture const *const texture;
};

void quad__copy_assets_to_gpu(
  struct gpu_api const *const gpu,
  struct quad *const qua
)

void quad__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct quad const *const qua
);

#endif