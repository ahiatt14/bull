#ifndef __BULL_PLAYER__
#define __BULL_PLAYER__

#include "tail.h"

#include "ecs.h"

#define PLAYER_ID 0
#define PLAYER_LEFT_LEG_INDEX 0
#define PLAYER_RIGHT_LEG_INDEX 1

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

EntityId create_firing_guide(
  ECS *const ecs
);

void pose_player_mech(
  GameTime time,
  Entity *const player,
  ECS *const ecs
);

#endif