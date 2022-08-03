#ifndef __BULL_WATER__
#define __BULL_WATER__

#include "tail.h"

#define OCEAN_KM_WIDE 20.0f

void water__init_mesh_data();
void water__copy_assets_to_gpu(
  struct gpu_api const *const gpu
);
// TODO: we'll want uv scroll accumulator
void water__update_waves(
  struct vec2 wind_speed,
  double delta_time,
  double seconds_since_creation,
  struct gpu_api const *const gpu
);
void water__draw(
  struct vec3 sunlight_direction,
  struct vec3 sunlight_color,
  struct camera const *const cam,
  struct gpu_api const *const gpu
);

#endif