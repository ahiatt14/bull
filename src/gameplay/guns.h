#ifndef __BULL_GUNS__
#define __BULL_GUNS__

#include <math.h>

#include "tail.h"

#include "ecs.h"

EntityId create_lvl0_cannonfire(
  struct Vec3 position,
  struct Vec3 direction,
  Seconds remainder,
  struct ECS *const ecs
);

#endif