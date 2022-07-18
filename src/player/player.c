#include <stdio.h>
#include <math.h>

#include "tail.h"

#include "player.h"
#include "bull_math.h"
#include "constants.h"
#include "gpu_helpers.h"

#include "bird_mesh.h"
#include "default_vert.h"
#include "solid_color_frag.h"
#include "normal_debug_frag.h"

#define PLAYER_SPEED 6
#define DEADZONE 0.2f

static uint8_t move_player_if_thats_what_they_want(
  double delta_time,
  struct vec2 const *const direction,
  struct player *const playr
);

static struct shader shared_healthy_shader;

static struct m4x4 shared_local_to_world;
static struct m3x3 shared_normals_local_to_world;

static struct vec2 shared_normalized_left_stick_direction;

// STATES

static void player_idle__update(
  double delta_time,
  struct gamepad_input gamepad,
  struct player *const playr,
  struct player_actions const *const actions
) {

  if (move_player_if_thats_what_they_want(
    delta_time,
    &gamepad.left_stick_direction,
    playr
  )) playr->input_state = PLAYER_INPUT_STATE__THRUSTING;;

  if (gamepad.left_and_right_triggers.y >= DEADZONE) {
    actions->start_auto_fire();
    playr->input_state = PLAYER_INPUT_STATE__AUTOFIRING;
  }
}

static void player_thrusting__update(
  double delta_time,
  struct gamepad_input gamepad,
  struct player *const playr,
  struct player_actions const *const actions
) {

  if (!move_player_if_thats_what_they_want(
    delta_time,
    &gamepad.left_stick_direction,
    playr
  )) playr->input_state = PLAYER_INPUT_STATE__IDLE;

  if (gamepad.left_and_right_triggers.y >= DEADZONE) {
    actions->start_auto_fire();
    playr->input_state = PLAYER_INPUT_STATE__AUTOFIRING;
  }
}

static void player_autofiring__update(
  double delta_time,
  struct gamepad_input gamepad,
  struct player *const playr,
  struct player_actions const *const actions
) {

  move_player_if_thats_what_they_want(
    delta_time,
    &gamepad.left_stick_direction,
    playr
  );

  if (gamepad.left_and_right_triggers.y < DEADZONE) {
    actions->stop_auto_fire();
    playr->input_state = PLAYER_INPUT_STATE__IDLE;
  }
}

static void player_healthy__update(
  double delta_time,
  struct player *const playr
) {
  return;
}

// PUBLIC API

void player__copy_assets_to_gpu(
  struct gpu_api const *const gpu
) {
  shared_healthy_shader.frag_shader_src = solid_color_frag_src;
  shared_healthy_shader.vert_shader_src = default_vert_src;
  gpu->copy_shader_to_gpu(&shared_healthy_shader);
  gpu->copy_static_mesh_to_gpu(&bird_mesh);
}

void player__init_state_lists(
  void (*input_state_updates[])(
    double delta_time,
    struct gamepad_input const *const gamepad,
    struct player *const playr,
    struct player_actions const *const actions 
  ),
  void (*effect_state_updates[])(
    double delta_time,
    struct player *const playr
  )
) {
  input_state_updates[PLAYER_INPUT_STATE__IDLE] = player_idle__update;
  input_state_updates[PLAYER_INPUT_STATE__THRUSTING] = player_thrusting__update;
  input_state_updates[PLAYER_INPUT_STATE__AUTOFIRING] =
    player_autofiring__update;

  effect_state_updates[PLAYER_EFFECT_STATE__HEALTHY] = player_healthy__update;
}

void player__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct player const *const playr
) {
  space__create_model(
    &WORLDSPACE,
    &playr->transform,
    &shared_local_to_world
  );
  space__create_normals_model(
    &shared_local_to_world,
    &shared_normals_local_to_world
  );
  gpu->select_shader(&shared_healthy_shader);
  gpu->set_fragment_shader_vec3(
    &shared_healthy_shader,
    "color",
    &COLOR_WHITE
  );
  gpu__set_mvp(
    &shared_local_to_world,
    &shared_normals_local_to_world,
    cam,
    &shared_healthy_shader,
    gpu
  );
  gpu->draw_mesh(&bird_mesh);
}

// HELPERS

static uint8_t move_player_if_thats_what_they_want(
  double delta_time,
  struct vec2 const *const direction,
  struct player *const playr
) {
  // TODO: maybe vector library should take vecs by value instead of ptr
  float mag = vec2__magnitude(direction);
  if (mag < DEADZONE) return 0;
  vec2__normalize(
    direction,
    &shared_normalized_left_stick_direction
  );
  playr->desired_position.x +=
    shared_normalized_left_stick_direction.x *
    PLAYER_SPEED * mag * mag * delta_time;
  playr->desired_position.z +=
    shared_normalized_left_stick_direction.y *
    PLAYER_SPEED * mag * mag * delta_time;
  return 1;
}