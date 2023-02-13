#ifndef __BULL_EXPLOSION__
#define __BULL_EXPLOSION__

#include "tail.h"

#include "ecs.h"

void explosions__copy_assets_to_gpu(
  struct GPU const *const gpu
);

void create_rpg_explosion(
  EntityId rocket,
  struct Vec3 camera_position,
  struct ECS *const ecs
);

#endif