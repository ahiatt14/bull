#ifndef __BULL_PLAYER__
#define __BULL_PLAYER__

#include "tail.h"

#include "ecs.h"

#define PLAYER_ID 0

#define LVL0_CANNON_AUTOFIRE_INTERVAL 0.10f

extern Shader player_shader;

void player__copy_assets_to_gpu(
  GPU const *const gpu
);

EntityId create_player(
  Vec3 position,
  void (*fire_lvl0_cannon)(
    EntityId player,
    Seconds remainder,
    ECS *const ecs
  ),
  ECS *const ecs
);

#endif