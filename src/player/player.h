#ifndef __BULL_PLAYER__
#define __BULL_PLAYER__

#include "tail.h"

#define PLAYER_INPUT_STATE__IDLE 0
#define PLAYER_INPUT_STATE__THRUSTING 1
#define PLAYER_INPUT_STATE__AUTOFIRING 2
#define PLAYER_INPUT_STATE__REELING 3

#define PLAYER_EFFECT_STATE__HEALTHY 0
#define PLAYER_EFFECT_STATE__REELING 1
#define PLAYER_EFFECT_STATE__BURNING 2

struct player {
  struct transform transform;
  struct vec3 projected_position;
  uint8_t input_state;
  uint8_t effect_state;
};

struct player_actions {
  void (*start_auto_fire)();
  void (*stop_auto_fire)();
  // void (*fire_special)();
};

// PUBLIC API

void player__set_default_state(
  struct player *const playr
);

void player__copy_assets_to_gpu(
  struct gpu_api const *const gpu
);

void player__update(
  double delta_time,
  struct gamepad_input gamepad,
  struct player_actions const *const actions,
  struct player *const playr
);

void player__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct player const *const playr
);

#endif