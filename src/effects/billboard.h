#ifndef __BULL_BILLBOARD__
#define __BULL_BILLBOARD__

#include "tail.h"

struct billboard {
  struct transform transform;
  const struct texture *texture;
  const struct shader *shader;
};

void billboard__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct billboard *const bb
);

#endif