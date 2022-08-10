#ifndef __BULL_BOUNCER_SEQUENCE__
#define __BULL_BOUNCER_SEQUENCE__

#include "bouncers.h"

struct bouncer_sequence {
  void (*defaults)(
    struct bouncer_grid *const *const grids
  );
  void (*update_grids)(
    double delta_time,
    double seconds_since_creation,
    struct bouncer_grid *const *const grids
  );
};

void two_swingy_arms__default(
  struct bouncer_grid *const *const grids
);

void two_swingy_arms__update_grids(
  double delta_time,
  double seconds_since_creation,
  struct bouncer_grid *const *const grids
);

#endif