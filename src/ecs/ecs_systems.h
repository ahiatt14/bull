#ifndef __BULL_ECS_SYSTEMS__
#define __BULL_ECS_SYSTEMS__

#include "tail.h"

#include "tail_helpers.h"

#include "ecs_definitions.h"

void ecs__move(
  struct GameTime time,
  struct ECS *const ecs
);

void ecs__timeout(
  struct GameTime time,
  struct ECS *const ecs
);

void ecs__scroll_uvs(
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