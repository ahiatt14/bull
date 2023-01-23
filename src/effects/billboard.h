#ifndef __BULL_BILLBOARD__
#define __BULL_BILLBOARD__

#include "tail.h"

void billboard__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct shader const *const shad,
  struct vec3 position,
  float scale
);

#endif