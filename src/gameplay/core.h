#ifndef __BULL_CORE__
#define __BULL_CORE__

struct core_state {
  struct transform transform;
};

void core__copy_assets_to_gpu(
  struct gpu_api const *const gpu
);

void core__update(
  double delta_time,
  struct core_state *const core
);

void core__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct core_state const *const core
);

#endif