#ifndef __BULL_WATER__
#define __BULL_WATER__

#include "tail.h"

#include "tail_helpers.h"

#define OCEAN_KM_WIDE 40.0f

void water__init_mesh_data();
void water__copy_assets_to_gpu(
  struct GPU const *const gpu
);
void water__update_waves(
  struct Vec2 wind_speed,
  struct GameTime time,
  struct GPU const *const gpu
);
void water__draw(
  struct Camera const *const cam,
  struct GPU const *const gpu
);

#endif