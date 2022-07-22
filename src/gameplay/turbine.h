#ifndef __BULL_TURBINE__
#define __BULL_TURBINE__

#include "tail.h"

#define TURBINE_BLADE_OFFSET { 0, 0.5f, -0.1f }

struct turbine {
  struct transform base_transform;
  struct transform blades_transform;
};

void turbine__copy_assets_to_gpu(
  struct gpu_api const *const gpu
);

void turbine__spin_blades(
  double delta_time,
  float degrees_per_second,
  struct turbine const *const turb
);

void turbine__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct turbine const *const turb
);

#endif