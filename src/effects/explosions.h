#ifndef __BULL_EXPLOSION__
#define __BULL_EXPLOSION__

#define MAX_EXPLOSIONS 10

#include "tail.h"

#include "billboard.h"

#include "tail_helpers.h"
#include "bull_math.h"

struct explosion {
  struct transform mushroom_cloud_transform;
  struct transform explosion_transform;
  struct billboard blink;
  double sec_since_activation;
};

struct explosions {
  struct explosion _ring_buffer[MAX_EXPLOSIONS];
  uint_fast16_t _tail;
  uint_fast16_t _active_count;
};

void explosions__copy_assets_to_gpu(
  struct gpu_api const *const gpu
);

void explosions__create(
  struct vec3 position,
  struct explosions *const explos
);

void explosions__destroy(
  uint_fast16_t from_start,
  struct explosions *const explos
);

void explosions__destroy_all(
  struct explosions *const explos
);

void explosions__update(
  struct gametime time,
  struct explosions *const explos
);

void explosions__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct explosions *const explos
);

#endif