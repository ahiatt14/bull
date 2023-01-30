#ifndef __BULL_ENTITY_FACTORY__
#define __BULL_ENTITY_FACTORY__

#include "tail.h"

#include "ecs.h"

EntityId create_rpg(
  struct Vec3 position,
  struct Vec3 velocity,
  void (*mark_entity_for_destruction)(EntityId),
  struct ECS *const ecs
);

#endif