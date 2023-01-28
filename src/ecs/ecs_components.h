#ifndef __BULL_ECS_COMPONENTS__
#define __BULL_ECS_COMPONENTS__

#include "tail.h"

#include "ecs_definitions.h"

void ecs__add_transform(
  EntityId id,
  struct transform t,
  struct ECS *const ecs
);

void ecs__add_draw_mesh(
  EntityId id,
  struct Draw drw,
  struct ECS *const ecs
);

void ecs__add_velocity(
  EntityId id,
  struct vec3 velocity,
  struct ECS *const ecs
);

void ecs__remove_velocity(
  EntityId id,
  struct ECS *const ecs
);

void ecs__add_timeout(
  EntityId id,
  struct Timeout t,
  struct ECS *const ecs
);

void ecs__remove_timeout(
  EntityId id,
  struct ECS *const ecs
);

#endif