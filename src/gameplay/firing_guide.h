#ifndef __BULL_FIRING_GUIDE__
#define __BULL_FIRING_GUIDE__

void firing_guide__copy_assets_to_gpu(
  struct GPU const *const gpu
);

void firing_guide__draw(
  struct Camera const *const cam,
  struct GPU const *const gpu,
  float arena_radius_worldspace,
  struct Vec3 player_position
);

#endif