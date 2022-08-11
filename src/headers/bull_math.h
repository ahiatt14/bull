#ifndef __BULL_MATH__
#define __BULL_MATH__

#include <stdint.h>

#include "tail.h"

const struct m4x4* camera__calculate_ortho(
  float r,
  float t,
  float n,
  float f,
  struct camera *const cam
);

char array_contains_u_i(
  unsigned int value,
  unsigned int *values,
  int count
);

struct vec2 vec2__turn_90_deg(
  uint8_t left,
  struct vec2 src
);

uint8_t is_moving_cw_around_world_up(
  struct vec3 position,
  struct vec3 previous_position
);

float find_cw_or_ccw_facing_around_world_up(
  struct vec3 position,
  struct vec3 previous_position
);

float rads_from_arc_len_and_radius(
  float arc_length,
  float radius
);

float loop_float(float v, float min, float max);

#endif