#ifndef __BULL_FLASHES__
#define __BULL_FLASHES__

void flashes__copy_assets_to_gpu(
  GPU const *const gpu
);

EntityId create_blue_pulse(
  Vec3 position,
  ECS *const ecs
);

void create_autocannon_muzzle_flash(
  EntityId weapon,
  Vec3 direction,
  Seconds remainder,
  ECS *const ecs
);

#endif