#ifndef __BULL_ECS_DRAWING__
#define __BULL_ECS_DRAWING__

#include "tail.h"

#include "ecs_types.h"

void ecs__prepare_entity_draw(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  EntityId id,
  ECS const *const ecs
);

void ecs__draw_mesh(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  Transform const *const hierarchy_transform,
  EntityId id,
  ECS const *const ecs
);

void ecs__draw_billboard(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  Transform const *const hierarchy_transform,
  EntityId id,
  ECS const *const ecs
);

void sort_alpha_entities(
  Vec3 camera_position,
  EntityId *alpha_entities,
  uint_fast16_t alpha_entity_count,
  ECS const *const ecs
);

#endif