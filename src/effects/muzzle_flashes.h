#ifndef __BULL_MUZZLE_FLASHES__
#define __BULL_MUZZLE_FLASHES__

#include "tail.h"

#include "ecs.h"

void muzzle_flashes__copy_assets_to_gpu(
  struct GPU const *const gpu
);

EntityId create_lvl0_muzzle_flash(
  struct Transform const *const source_transform,
  struct ECS *const ecs
);

#endif