#ifndef __BULL_CLOUD_WALL__
#define __BULL_CLOUD_WALL__

#include "tail.h"

#include "ecs.h"

#include "tail_helpers.h"

void cloud_wall__copy_assets_to_gpu(
  GPU const *const gpu
);

void create_cloud_wall(
  ECS *const ecs
);

#endif