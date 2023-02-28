#ifndef __BULL_SPARKS__
#define __BULL_SPARKS__

#include "tail.h"

#include "ecs.h"

void create_sparks(
  Vec3 position,
  Vec3 source_velocity,
  uint_fast8_t count,
  struct ECS *const ecs
);

#endif