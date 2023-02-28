#ifndef __BULL_PLAYER__
#define __BULL_PLAYER__

#include "tail.h"

#include "ecs.h"

#define PLAYER_ID 0

#define LVL0_CANNON_AUTOFIRE_INTERVAL 0.10f

extern struct Shader player_shader;

void player__copy_assets_to_gpu(
  struct GPU const *const gpu
);

EntityId create_player(
  struct Vec3 position,
  void (*fire_lvl0_cannon)(
    EntityId player,
    Seconds remainder,
    struct ECS *const ecs
  ),
  struct ECS *const ecs
);

#endif