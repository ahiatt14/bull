#ifndef __BULL_ECS_COMPONENTS__
#define __BULL_ECS_COMPONENTS__

#include "tail.h"

#include "ecs_definitions.h"

uint_fast8_t lacks_configuration(
  uint_fast16_t necessary_configuration,
  uint_fast16_t entity_configuration
);

uint_fast8_t has_component(
  uint_fast16_t necessary_component,
  uint_fast16_t entity_configuration
);

void ecs__add_transform(
  EntityId id,
  struct Transform t,
  struct ECS *const ecs
);

void ecs__add_draw_mesh(
  EntityId id,
  struct Draw drw,
  struct ECS *const ecs
);

void ecs__add_velocity(
  EntityId id,
  struct Vec3 velocity,
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