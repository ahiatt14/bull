#include "tail.h"

#include "ecs.h"

void plume_plant__copy_assets_to_gpu(
  GPU const *const gpu
);

EntityId create_plume_plant(
  Vec3 position,
  ECS *const ecs
);