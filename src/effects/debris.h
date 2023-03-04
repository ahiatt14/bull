#ifndef __BULL_DEBRIS__
#define __BULL_DEBRIS__

#include "tail.h"

#include "ecs.h"

void create_debris(
  Vec3 position,
  Vec3 source_velocity,
  uint_fast8_t count,
  ECS *const ecs
);

#endif