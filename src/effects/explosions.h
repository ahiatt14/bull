#ifndef __BULL_EXPLOSION__
#define __BULL_EXPLOSION__

#include "tail.h"

#include "ecs.h"

void explosions__copy_assets_to_gpu(
  struct GPU const *const gpu
);

EntityId create_rpg_explosion(
  struct Vec3 position,
  void (*mark_entity_for_destruction)(
    EntityId id,
    Seconds remainder,
    struct ECS *const ecs
  ),
  struct ECS *const ecs
);

#endif