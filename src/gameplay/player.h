#ifndef __BULL_PLAYER__
#define __BULL_PLAYER__

#include "tail.h"

#include "tail_helpers.h"

#define PLAYER_INPUT_STATE__IDLE 0
#define PLAYER_INPUT_STATE__THRUSTING 1
#define PLAYER_INPUT_STATE__AUTOFIRING 2
#define PLAYER_INPUT_STATE__REELING 3
typedef uint8_t player_input_state;

#define PLAYER_EFFECT_STATE__HEALTHY 0
#define PLAYER_EFFECT_STATE__REELING 1
typedef uint8_t player_effect_state;

#define PLAYER_FACING_COEF__CCW 0
#define PLAYER_FACING_COEF__CW 1
#define PLAYER_FACING_COEF__IN 2
typedef uint8_t player_facing_coef;

struct player {
  struct transform transform;
  struct vec3 previous_position;
  struct vec3 projected_position;
  player_input_state input_state;
  player_effect_state effect_state;
  player_facing_coef facing_coef;
  uint8_t facing_locked;
  uint8_t level;
};

struct player_actions {
  void (*start_autofire)(
    struct gametime time,
    struct player const *const playr
  );
  void (*stop_autofire)(
    struct gametime time,
    struct player const *const playr
  );
  // void (*fire_special)();
};

// PUBLIC API

void player__reset_state(
  struct player *const playr
);

void player__copy_assets_to_gpu(
  struct gpu_api const *const gpu
);

void player__update(
  struct gametime time,
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