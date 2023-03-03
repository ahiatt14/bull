#ifndef __BULL_ECS_DRAWING__
#define __BULL_ECS_DRAWING__

#include "tail.h"

#include "ecs_types.h"

void set_textures(
  Entity const *const entity,
  GPU const *const gpu
);

void ecs__draw_mesh(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  EntityId id,
  ECS const *const ecs
);

void ecs__draw_billboard(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  EntityId id,
  ECS const *const ecs
);

#endif