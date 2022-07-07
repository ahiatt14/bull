#ifndef __BULL_PLAYER__
#define __BULL_PLAYER__

#include "tail.h"

#define PLAYER_STATE__IDLE 0
#define PLAYER_STATE__THRUSTING 1
#define PLAYER_STATE__COASTING 2
#define PLAYER_STATE__DYING 3

struct player_state;

struct player {
  struct transform transform;
  struct player_state *current_state;
};

struct player_state {
  void (*update)(
    double delta_time,
    struct gamepad_input const *const input,
    struct player_state **const states,
    struct player *const playr
  );
  void (*draw)(
    struct camera const *const cam,
    struct gpu_api const *const gpu,
    struct player const *const playr
  );
};

void player__copy_assets_to_gpu(struct gpu_api const *const gpu);
void player__init_states(struct player_state **const states);

#endif