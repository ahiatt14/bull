#ifndef __BULL_FLASHES__
#define __BULL_FLASHES__

void flashes__copy_assets_to_gpu(
  GPU const *const gpu
);

EntityId create_blue_pulse(
  Vec3 position,
  ECS *const ecs
);

#endif