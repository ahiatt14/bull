#ifndef __BULL_PLAYER__
#define __BULL_PLAYER__

#include "tail.h"

#include "tail_helpers.h"

#define PLAYER_INPUT_STATE__IDLE 0
#define PLAYER_INPUT_STATE__THRUSTING 1
#define PLAYER_INPUT_STATE__AUTOFIRING 2
#define PLAYER_INPUT_STATE__REELING 3
#define PLAYER_INPUT_STATE__FLIPPING 4
typedef uint8_t player_input_state;

#define PLAYER_EFFECT_STATE__HEALTHY 0
#define PLAYER_EFFECT_STATE__REELING 1
typedef uint8_t player_effect_state;

struct Player {
  struct transform transform;
  struct vec3 previous_position;
  struct vec3 projected_position;
  player_input_state input_state;
  player_effect_state effect_state;
  uint8_t level;
};

struct PlayerActions {
  void (*start_autofire)(
    struct GameTime time,
    struct Player const *const playr
  );
  void (*stop_autofire)(
    struct GameTime time,
    struct Player const *const playr
  );
  // void (*fire_special)();
};

// PUBLIC API

void player__reset_state(
  struct Player *const playr
);

void player__copy_assets_to_gpu(
  struct gpu_api const *const gpu
);

void player__update(
  struct GameTime time,
  struct gamepad_input gamepad,
  struct PlayerActions const *const actions,
  struct Player *const playr
);

void player__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct Player const *const playr
);

#endif