#ifndef __BULL_FIREBALLS__
#define __BULL_FIREBALLS__

#include "tail.h"

#include "tail_helpers.h"

// #define MAX_FIREBALL_COUNT 69

// fireballs.c or action.c?
// static struct fireball fireballs[MAX_FIREBALL_COUNT];

struct fireball {
  float cw_deg;
  float starting_cw_deg;
  float origin_radius;
  double seconds_since_activation;
  int8_t cw_coef; // 1 for ccw, -1 for cw (?)
};

void autofire_lvl1_fireballs(
  struct gametime time,
  struct vec3 source
);

void autofire_lvl2_fireballs(
  struct gametime time,
  struct vec3 source
);

void fireballs__revolve(
  struct gametime time,
  float deg_per_sec,
  struct fireball *const fireballs
);

#endif