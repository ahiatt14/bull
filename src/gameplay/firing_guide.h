#ifndef __BULL_FIRING_GUIDE__
#define __BULL_FIRING_GUIDE__

void firing_guide__copy_assets_to_gpu(
  struct gpu_api const *const gpu
);

void firing_guide__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct vec3 player_position
);

#endif