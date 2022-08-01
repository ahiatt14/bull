#ifndef __BULL_QUAD__
#define __BULL_QUAD__

#include "tail.h"

struct quad {
  float aspect_ratio;
  struct transform transform;
  struct texture *texture;
};

void quad__copy_assets_to_gpu(
  struct gpu_api const *const gpu,
  struct quad *const qua
);

void quad__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct quad const *const qua
);

#endif