#ifndef __BULL_FIREBALLS__
#define __BULL_FIREBALLS__

#include "tail.h"

#include "tail_helpers.h"
#include "bull_math.h"

#define MAX_FIREBALLS 20

struct fireball {
  struct battlefield_pos position;
  double sec_since_activation;
};

struct fireballs {
  struct fireball _ring_buffer[MAX_FIREBALLS];
  unsigned int _tail;
  unsigned int _active_count;
};

void fireballs__copy_assets_to_gpu(
  struct gpu_api const *const gpu
);

void fireballs__deactivate_all(
  struct fireballs *const fbs
);

void fireballs__move(
  struct gametime time,
  float world_unit_per_second,
  double lifespan_in_seconds,
  void (*on_max_radius_contact)(
    uint_fast16_t from_start_index,
    struct fireball *const fb
  ),
  struct fireballs *const fbs
);

void fireballs__activate(
  struct battlefield_pos bfpos,
  struct fireballs *const fbs
);

void fireballs__deactivate(
  uint_fast16_t from_start,
  struct fireballs *const fbs
);

void fireballs__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct fireballs *const fbs
);

#endif