#ifndef __BULL_ECS_DRAWING__
#define __BULL_ECS_DRAWING__

#include "tail.h"

#include "ecs_types.h"

void set_textures(
  struct Entity const *const entity,
  GPU const *const gpu
);

void ecs__draw_mesh(
  struct GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  struct Entity const *const entity
);

void ecs__draw_billboard(
  struct GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  struct Entity const *const entity
);

#endif