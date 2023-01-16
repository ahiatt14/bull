#ifndef __BULL_MATH__
#define __BULL_MATH__

#include <stdint.h>

#include "tail.h"

// NOTE: degrees are assumed to move ccw from -Z around WORLDSPACE.up
struct battlefield_pos {
  float radius;
  float degrees;
};

struct revolution {
  float starting_deg;
  double sec_elapsed_when_activated;
};

float battlefield_deg_from_world_pos(
  struct vec3 position
);

struct vec3 battlefield_to_world_pos(
  struct battlefield_pos position
);

struct battlefield_pos world_to_battlefield_pos(
  struct vec3 position
);

const struct m4x4* camera__calculate_ortho(
  float r,
  float t,
  float n,
  float f,
  struct camera *const cam
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

struct vec3 slide_along_radius_around_world_origin(
  float radius,
  struct vec3 projected_pos,
  struct vec3 pos
);

struct vec3 vec3__lerp(
  struct vec3 t0,
  struct vec3 t1,
  float p
);

#endif