#ifndef __BULL_TURBINE__
#define __BULL_TURBINE__

#include "tail.h"

struct turbine {
  struct transform transform;
  float blades_rotation_in_deg;
};

void turbine__copy_assets_to_gpu(
  struct gpu_api const *const gpu
);

void turbine__spin_blades(
  double delta_time,
  float degrees_per_second,
  struct turbine *const turb
);

void turbine__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  // struct vec2 shadow_uv,
  struct vec3 sunlight_direction,
  struct vec3 sunlight_color,
  struct turbine const *const turb
);

#endif