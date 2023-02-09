#ifndef __BULL_GUNS__
#define __BULL_GUNS__

#include <math.h>

#include "tail.h"

#include "ecs.h"

void guns__copy_assets_to_gpu(
  struct GPU const *const gpu
);

EntityId create_lvl0_cannonfire(
  struct Vec3 position,
  struct Vec3 direction,
  void (*on_timeout)(
    EntityId id,
    Seconds remainder,
    struct ECS *const ecs
  ),
  struct ECS *const ecs
);

#endif