#ifndef __BULL_MINES__
#define __BULL_MINES__

#include "tail.h"

#include "ecs.h"

void mines__init_scene_callbacks(
  void (*on_shot)(
    EntityId mine,
    EntityId projectile,
    struct ECS *const ecs
  )
);

void mines__copy_assets_to_gpu(
  struct GPU const *const gpu
);

void mines__create_pattern_0(
  struct ECS *const ecs
);

void mines__create_pattern_1(
  struct ECS *const ecs
);

void create__mine(
  struct Vec3 spawn_start,
  struct Vec3 battlefield_start,
  uint_fast8_t ccw,
  struct ECS *const ecs
);

#endif