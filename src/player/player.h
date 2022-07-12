#ifndef __BULL_PLAYER__
#define __BULL_PLAYER__

#include "tail.h"

#define PLAYER_STATE__IDLE 0
#define PLAYER_STATE__THRUSTING 1
#define PLAYER_STATE__CCW_TO_CW 2
#define PLAYER_STATE__CW_TO_CCW 3
// #define PLAYER_STATE__COASTING 
// #define PLAYER_STATE__DYING 

struct player_state;

struct player {
  struct transform transform;
  struct vec3 previous_position;
  uint8_t facing_cw; // TODO: is this redundant?
  // does the transform contain info enough to derive from?
  uint8_t current_state;
};

struct player_state {
  void (*update)(
    double delta_time,
    struct gamepad_input const *const input,
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