#ifndef __BULL_HIERARCHIES__
#define __BULL_HIERARCHIES__

#include "tail.h"

#include "ecs.h"

void add_parents_transforms(
  Transform *const total_transform,
  Entity const *const child,
  ECS const *const ecs
);

#endif