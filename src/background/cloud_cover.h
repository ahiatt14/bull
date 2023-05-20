#ifndef __BULL_CLOUD_COVER__
#define __BULL_CLOUD_COVER__

#include "tail.h"

#include "ecs.h"

#include "tail_helpers.h"

void cloud_cover__copy_assets_to_gpu(
  GPU const *const gpu
);

void create_cloud_cover(
  ECS *const ecs
);

#endif