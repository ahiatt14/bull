#ifndef __BULL_LIGHTING__
#define __BULL_LIGHTING__

#include "tail.h"

#include "ecs.h"

#define MAX_POINT_LIGHTS 20

typedef struct BULLLIGHTING {
  EntityId point_lights[MAX_POINT_LIGHTS];
  uint_fast8_t point_light_count;
  Transform hierarchy_transforms[MAX_POINT_LIGHTS];
} Lighting;

#endif