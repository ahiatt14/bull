#ifndef __BULL_OCEAN__
#define __BULL_OCEAN__

#include "tail.h"
#include "ecs.h"

void water__copy_assets_to_gpu(
  struct GPU const *const gpu
);

EntityId create_water(
  struct ECS *const ecs
);

#endif