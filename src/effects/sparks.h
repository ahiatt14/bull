#ifndef __BULL_SPARKS__
#define __BULL_SPARKS__

#include "tail.h"

#include "ecs.h"

void sparks__copy_assets_to_gpu(
  struct GPU const *const gpu
);

void create_sparks(
  struct Vec3 position,
  struct Vec3 direction,
  uint_fast8_t count,
  struct ECS *const ecs
);

#endif