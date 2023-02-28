#ifndef __BULL_ECS_ENTITIES__
#define __BULL_ECS_ENTITIES__

#include "tail.h"

#include "ecs_types.h"

EntityId ecs__create_entity(
  ECS *const ecs
);

void ecs__mark_for_destruction(
  EntityId id,
  ECS *const ecs
);

void ecs__destroy_marked_entities(
  ECS *const ecs
);

#endif