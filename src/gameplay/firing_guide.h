#ifndef __BULL_FIRING_GUIDE__
#define __BULL_FIRING_GUIDE__

void firing_guide__copy_assets_to_gpu(
  GPU const *const gpu
);

void firing_guide__draw(
  Camera const *const cam,
  GPU const *const gpu,
  float arena_radius_worldspace,
  Vec3 player_position
);

#endif