#ifndef __BULL_MINES__
#define __BULL_MINES__

#include "tail.h"

#include "ecs.h"

void mines__copy_assets_to_gpu(
  struct GPU const *const gpu
);

void create__mine(
  struct Vec3 spawn_start,
  struct Vec3 battlefield_start,
  uint_fast8_t ccw,
  struct ECS *const ecs
);

#endif