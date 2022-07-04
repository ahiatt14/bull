#include "tail.h"

#include "arena_scene.h"
#include "constants.h"

double delta_time;
double start_time;

void arena__init(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu
) {

}

static double seconds_since_creation;
void arena__tick(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu,
  struct scene const *const *const scenes,
  void switch_scene(struct scene const *const new_scen)
) {
  seconds_since_creation = window->get_seconds_since_creation();
  delta_time = seconds_since_creation - start_time;
  if (delta_time > DELTA_TIME_CAP) delta_time = DELTA_TIME_CAP;
  start_time = seconds_since_creation;

  // UPDATE

  // DRAW
  gpu->clear(&COLOR_RED);
}