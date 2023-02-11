#ifndef __BULL_SPARKS__
#define __BULL_SPARKS__

#include "tail.h"

#include "ecs.h"

void create_sparks(
  struct Vec3 position,
  struct Vec3 direction,
  uint_fast8_t count,
  void (*mark_entity_for_destruction)(
    EntityId id,
    Seconds remainder,
    struct ECS *const ecs
  ),
  struct ECS *const ecs
);

#endif