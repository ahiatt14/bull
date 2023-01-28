#ifndef __BULL_ECS__
#define __BULL_ECS__

#include "ecs_definitions.h"

EntityId ecs__create_entity(
  struct ECS *const ecs
);

void ecs__destroy_entity(
  EntityId id,
  struct ECS *const ecs
);

#endif