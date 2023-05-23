#ifndef __BULL_DRONE__
#define __BULL_DRONE__

#include "tail.h"

#include "ecs.h"

void drones__init_scene_callbacks(  
);

void drones__copy_assets_to_gpu(
  GPU const *const gpu 
);

EntityId create_drone(
  Vec3 position,
  ECS *const ecs
);

#endif