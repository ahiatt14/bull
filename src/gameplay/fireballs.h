#ifndef __BULL_FIREBALLS__
#define __BULL_FIREBALLS__

#include "tail.h"

#include "tail_helpers.h"
#include "bull_math.h"

void fireballs__copy_assets_to_gpu(
  struct gpu_api const *const gpu
);

void fireballs__deactivate_all();

void fireballs__move(
  struct gametime time,
  float world_unit_per_second,
  float max_radius
);

void fireballs__revolve(
  struct gametime time,
  double sec_per_revolution
);

void fireballs__activate_fireball(
  struct battlefield_pos bfpos
);

void fireballs__deactivate_fireball(
  int_fast16_t index_to_deactivate
);

void fireballs__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu
);

#endif