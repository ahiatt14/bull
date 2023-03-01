#ifndef __BULL_OCEAN_SKYBOX__
#define __BULL_OCEAN_SKYBOX__

#include "tail.h"

#include "ecs.h"

#include "tail_helpers.h"

void ocean_skybox__copy_assets_to_gpu(
  GPU const *const gpu
);

void draw_ocean_skybox(
  Camera const *const camera,
  GPU const *const gpu
);

#endif