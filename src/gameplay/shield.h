#ifndef __BULL_SHIELD__
#define __BULL_SHIELD__

#include "tail.h"

#include "ecs.h"
#include "constants.h"

void shield__copy_assets_to_gpu(
  GPU const *const gpu
);

void shield__create_free_shield(
  Vec3 position,
  float radius,
  void (*start_script)(
    EntityId id,
    Seconds remainder,
    ECS const *const ecs
  ),
  ECS const *const ecs
);

#endif