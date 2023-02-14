#ifndef __BULL_ECS_DRAWING__
#define __BULL_ECS_DRAWING__

#include "tail.h"

#include "ecs_types.h"

void set_mvp(
  struct Transform const *const transform,
  struct Camera const *const camera,
  struct GPU const *const gpu
);

void ecs__draw_mesh(
  struct GameTime time,
  struct Camera const *const camera,
  struct GPU const *const gpu,
  EntityId id,
  struct ECS const *const ecs
);

void ecs__draw_billboard(
  struct GameTime time,
  struct Camera const *const camera,
  struct GPU const *const gpu,
  EntityId id,
  struct ECS const *const ecs
);

#endif