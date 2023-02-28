#ifndef __BULL_DRONE__
#define __BULL_DRONE__

void drones__init_scene_callbacks(  
);

void drones__copy_assets_to_gpu(
  struct GPU const *const gpu 
);

EntityId create_drone(
  struct ECS *const ecs
);

#endif