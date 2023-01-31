#ifndef __BULL_ENTITY_FACTORY__
#define __BULL_ENTITY_FACTORY__

#include "tail.h"

#include "ecs.h"

EntityId deploy_rpg(
  struct Vec3 position,
  void (*on_deployed)(
    EntityId id,
    double remainder_in_seconds,
    struct ECS *const ecs
  ),
  struct ECS *const ecs
);

#endif