#ifndef __BULL_AFTERIMAGES__
#define __BULL_AFTERIMAGES__

#include "tail.h"

#include "ecs.h"

#include "constants.h"

void afterimages__copy_assets_to_gpu(
  GPU const *const gpu
);

void create_player_afterimage(
  EntityId player,
  Seconds remainder,
  ECS *const ecs
);

#endif