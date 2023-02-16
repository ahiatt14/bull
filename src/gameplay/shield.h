#ifndef __BULL_SHIELD__
#define __BULL_SHIELD__

#include "tail.h"

#include "ecs.h"
#include "constants.h"

void shield__copy_assets_to_gpu(
  struct GPU const *const gpu
);

void shield__create_free_shield(
  struct Vec3 position,
  float radius,
  void (*start_script)(
    EntityId id,
    Seconds remainder,
    struct ECS const *const ecs
  ),
  struct ECS const *const ecs
);

#endif