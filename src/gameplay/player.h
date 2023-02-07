#ifndef __BULL_PLAYER__
#define __BULL_PLAYER__

#include "tail.h"

#include "ecs.h"

void player__copy_assets_to_gpu(
  struct GPU const *const gpu
);

EntityId create_player(
  struct Vec3 position,
  struct ECS *const ecs
);

#endif