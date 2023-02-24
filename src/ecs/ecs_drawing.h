#ifndef __BULL_ECS_DRAWING__
#define __BULL_ECS_DRAWING__

#include "tail.h"

#include "ecs_types.h"

void set_textures(
  struct Entity const *const entity,
  struct GPU const *const gpu
);

void ecs__draw_mesh(
  struct GameTime time,
  struct Camera const *const camera,
  struct GPU const *const gpu,
  struct Entity const *const entity
);

void ecs__draw_billboard(
  struct GameTime time,
  struct Camera const *const camera,
  struct GPU const *const gpu,
  struct Entity const *const entity
);

#endif