#ifndef __BULL_ARENA__
#define __BULL_ARENA__

struct arena_state {
  struct transform transform;
};

void arena__copy_assets_to_gpu(
  struct gpu_api const *const gpu
);

void arena__update(
  double delta_time,
  struct arena_state *const arena
);

void arena__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct arena_state const *const arena
);

#endif