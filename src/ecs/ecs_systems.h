#ifndef __BULL_ECS_SYSTEMS__
#define __BULL_ECS_SYSTEMS__

#include "tail.h"
#include "tail_helpers.h"

#include "ecs_types.h"

void ecs__control_player(
  struct GameTime time,
  struct Gamepad gamepad,
  struct ControllerActions const *const actions,
  struct Entity *const player
);

void ecs__move(
  struct GameTime time,
  struct ECS *const ecs
);

void ecs__timeout(
  struct GameTime time,
  struct ECS *const ecs
);

void ecs__repeat(
  struct GameTime time,
  struct ECS *const ecs
);

void ecs__scroll_uvs(
  struct GameTime time,
  struct ECS *const ecs
);

void ecs__lerp_vec3(
  struct GameTime time,
  struct ECS *const ecs
);

void ecs__lerp_revolve(
  struct GameTime time,
  struct ECS *const ecs
);

void ecs__look_at_center(
  struct GameTime time,
  struct ECS *const ecs
);

void ecs__draw(
  struct GameTime time,
  struct Camera const *const cam,
  struct GPU const *const gpu,
  struct ECS *const ecs
);

#endif