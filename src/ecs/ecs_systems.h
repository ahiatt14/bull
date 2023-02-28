#ifndef __BULL_ECS_SYSTEMS__
#define __BULL_ECS_SYSTEMS__

#include "tail.h"
#include "tail_helpers.h"

#include "ecs_types.h"

void ecs__control_player(
  GameTime time,
  Gamepad gamepad,
  ControllerActions const *const actions,
  Entity *const player
);

void ecs__gravity(
  GameTime time,
  ECS *const ecs
);

void ecs__move(
  GameTime time,
  ECS *const ecs
);

void ecs__timeout(
  GameTime time,
  ECS *const ecs
);

void ecs__repeat(
  GameTime time,
  ECS *const ecs
);

void ecs__scroll_uvs(
  GameTime time,
  ECS *const ecs
);

void ecs__lerp_vec3(
  GameTime time,
  ECS *const ecs
);

void ecs__lerp_revolve(
  GameTime time,
  ECS *const ecs
);

void ecs__lerp_rotation(
  GameTime time,
  ECS *const ecs
);

void ecs__look_at_center(
  GameTime time,
  ECS *const ecs
);

void ecs__check_projectile_radius_collisions(
  GameTime time,
  ECS *const ecs
);

void ecs__check_pickup_radius_collisions(
  GameTime time,
  ECS *const ecs
);

void ecs__draw(
  GameTime time,
  Camera const *const cam,
  GPU const *const gpu,
  ECS *const ecs
);

#endif