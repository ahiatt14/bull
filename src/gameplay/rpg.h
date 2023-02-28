#ifndef __BULL_ENTITY_FACTORY__
#define __BULL_ENTITY_FACTORY__

#include "tail.h"

#include "ecs.h"

EntityId deploy_rpg(
  Vec3 position,
  void (*on_rpg_deployed)(
    EntityId id,
    Seconds remainder,
    ECS *const ecs
  ),
  ECS *const ecs
);

void propel_rpg(
  EntityId rocket,
  Seconds remainder,
  void (*on_rpg_timer_up)(
    EntityId id,
    Seconds remainder,
    ECS *const ecs
  ),
  ECS *const ecs
);

void create_rpg_thruster_blink(
  EntityId rocket,
  ECS *const ecs
);

#endif