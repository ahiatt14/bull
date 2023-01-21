#ifndef __BULL_BILLBOARD__
#define __BULL_BILLBOARD__

#include "tail.h"

struct billboard {
  struct transform transform;
  const struct texture *texture;
};

void billboard__copy_shared_assets_to_gpu(
  struct gpu_api const *const gpu
);

void billboard__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct billboard *const bb
);

#endif