#ifndef __BULL_FIREBALLS__
#define __BULL_FIREBALLS__

#include "tail.h"

#include "tail_helpers.h"
#include "bull_math.h"

struct fireball {
  struct battlefield_pos position;
  float starting_battlefield_deg;
  float seconds_since_activation;
  int8_t ccw_coefficient; // 1 for ccw, -1 for cw (?)
};

void fireballs__copy_assets_to_gpu(
  struct gpu_api const *const gpu
);

void fireballs__reset_state();

void fireballs__revolve(
  struct gametime time,
  float deg_per_sec
);

void fireballs__activate_fireball(
  struct vec3 position,
  int_fast8_t ccw_coefficient
);

void fireballs__deactivate(
  int_fast16_t index_to_deactivate
);

void fireballs__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu
);

#endif