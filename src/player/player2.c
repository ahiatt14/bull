#include <stdio.h>
#include <math.h>

#include "tail.h"

#include "bull_math.h"
#include "constants.h"
#include "gpu_helpers.h"
#include "player.h"

#include "bird_mesh.h"
#include "default_vert.h"
#include "solid_color_frag.h"
#include "normal_debug_frag.h"

#define PLAYER_SPEED 6
#define DEADZONE 0.2f

#define PLAYER_INPUT_STATE__IDLE 0
#define PLAYER_INPUT_STATE__THRUSTING 1
#define PLAYER_INPUT_STATE__REELING 2

#define PLAYER_EFFECT_STATE__HEALTHY 0
#define PLAYER_EFFECT_STATE__REELING 1
#define PLAYER_EFFECT_STATE__BURNING 2

static struct shader shared_healthy_shader;

static struct m4x4 shared_local_to_world;
static struct m3x3 shared_normals_local_to_world;

static struct vec2 shared_normalized_left_stick_direction;

// TODO: this goes in action scene file
// when the player leaves the arena
// the action scene will swap these out
// for different versions that shoot inward
// instead of around
struct player_actions {
  void (*start_main_auto_fire)();
  void (*stop_main_auto_fire)();
  void (*fire_special)();
};

struct player {
  struct drawable_mesh mesh;
  struct transform transform;
  struct vec3 previous_position;
  uint8_t current_move_state;
  uint8_t current_effect_state;
};

static void player_idle__update(
  double delta_time,
  struct gamepad_input const *const gamepad,
  struct player *const playr,
  struct player_actions const *const actions
);

static void player_thrusting__update(
  double delta_time,
  struct gamepad_input const *const gamepad,
  struct player *const playr,
  struct player_actions const *const actions
);

static void player_healthy__update(
  double delta_time,
  struct player *const playr
);

static void player_healthy__draw(
  struct camera *const cam, 
  struct player const *const playr,
  struct gpu_api const *const gpu
);

// STATES

// TODO: I think these go in the action scene file?
static void (*player_input_state_updates)(
  double delta_time,
  struct gamepad_input const *const gamepad,
  struct player *const playr,
  struct player_actions const *const actions 
)[PLAYER_MOVE_STATE_COUNT] = {
  player_idle__update,
  player_thrusing__update
};

static void (*player_effect_state_updates)(
  double delta_time,
  struct player *const playr
)[PLAYER_EFFECT_STATE_COUNT] = {
  player_healthy__update
};

static void (*player_effect_state_draws)(
  struct camera *const cam, 
  struct player const *const playr,
  struct gpu_api const *const gpu
)[PLAYER_EFFECT_STATE_COUNT] = {
  player_healthy__draw
};

static void player_idle__update(
    double delta_time,
    struct gamepad_input const *const gamepad,
    struct player *const playr,
    void (*)
) {
  if (vec2__magnitude(&gamepad->left_stick_direction) > DEADZONE) {
    move_player(
      playr,
      &gamepad->left_stick_direction,
      delta_time
    );
    playr->current_state = PLAYER_INPUT_STATE__THRUSTING;
  }
}

static void player_thrusting__update(
    double delta_time,
    struct gamepad_input const *const gamepad,
    struct player *const playr
) {
  if (vec2__magnitude(&gamepad->left_stick_direction) > DEADZONE) {
    move_player(
      playr,
      &gamepad->left_stick_direction,
      delta_time
    );
  } else {
    playr->current_state = PLAYER_STATE__IDLE;
  }
  save_player_history(playr);
}

// HELPERS

static void move_player(
  struct player *const playr,
  struct vec2 const *const direction,
  double delta_time
) {
  float mag = vec2__magnitude(direction);
  vec2__normalize(
    direction,
    &shared_normalized_left_stick_direction
  );
  playr->transform.position.x +=
    shared_normalized_left_stick_direction.x *
    PLAYER_SPEED * mag * mag * delta_time;
  playr->transform.position.z +=
    shared_normalized_left_stick_direction.y *
    PLAYER_SPEED * mag * mag * delta_time;
}