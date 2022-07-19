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

// CONSTANTS

#define PLAYER_INPUT_STATE_COUNT 3
#define PLAYER_EFFECT_STATE_COUNT 1

#define PLAYER_SPEED 6
#define TRIGGER_DEADZONE 0.5f
#define STICK_DEADZONE 0.2f

// FORWARD DECS

static uint8_t project_player_position(
  double delta_time,
  struct vec2 direction,
  struct player *const playr
);

// LOCALS

static struct shader shared_healthy_shader;

static struct m4x4 shared_local_to_world;
static struct m3x3 shared_normals_local_to_world;

static struct vec2 shared_normalized_left_stick_direction;

// STATE STUFF

static void player_idle__update(
  double delta_time,
  struct gamepad_input gamepad,
  struct player_actions const *const actions,
  struct player *const playr
) {

  if (project_player_position(
    delta_time,
    gamepad.left_stick_direction,
    playr
  )) playr->input_state = PLAYER_INPUT_STATE__THRUSTING;

  if (gamepad.left_and_right_triggers.y >= TRIGGER_DEADZONE) {
    actions->start_auto_fire();
    playr->input_state = PLAYER_INPUT_STATE__AUTOFIRING;
  }
}

static void player_thrusting__update(
  double delta_time,
  struct gamepad_input gamepad,
  struct player_actions const *const actions,
  struct player *const playr
) {

  if (!project_player_position(
    delta_time,
    gamepad.left_stick_direction,
    playr
  )) playr->input_state = PLAYER_INPUT_STATE__IDLE;

  if (gamepad.left_and_right_triggers.y >= TRIGGER_DEADZONE) {
    actions->start_auto_fire();
    playr->input_state = PLAYER_INPUT_STATE__AUTOFIRING;
  }
}

static void player_autofiring__update(
  double delta_time,
  struct gamepad_input gamepad,
  struct player_actions const *const actions,
  struct player *const playr
) {

  project_player_position(
    delta_time,
    gamepad.left_stick_direction,
    playr
  );

  if (gamepad.left_and_right_triggers.y < TRIGGER_DEADZONE) {
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

void (*player_input_state_updates[PLAYER_INPUT_STATE_COUNT])(
  double delta_time,
  struct gamepad_input gamepad,
  struct player_actions const *const actions,
  struct player *const playr
) = {
  player_idle__update,
  player_thrusting__update,
  player_autofiring__update
};

void (*player_effect_state_updates[PLAYER_EFFECT_STATE_COUNT])(
  double delta_time,
  struct player *const playr
) = {
  player_healthy__update
};

// PUBLIC API

void player__copy_assets_to_gpu(
  struct gpu_api const *const gpu
) {
  shared_healthy_shader.frag_shader_src = solid_color_frag_src;
  shared_healthy_shader.vert_shader_src = default_vert_src;
  gpu->copy_shader_to_gpu(&shared_healthy_shader);
  gpu->copy_static_mesh_to_gpu(&bird_mesh);
}

void player__update(
  double delta_time,
  struct gamepad_input gamepad,
  struct player_actions const *const actions,
  struct player *const playr
) {
  player_input_state_updates[playr->input_state](
    delta_time,
    gamepad,
    actions,
    playr
  );
  // TODO: add effect update call
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

static uint8_t project_player_position(
  double delta_time,
  struct vec2 direction,
  struct player *const playr
) {
  // TODO: maybe vector library should take vecs by value instead of ptr
  float mag = vec2__magnitude(direction);
  if (mag < STICK_DEADZONE) return 0;
  shared_normalized_left_stick_direction = vec2__normalize(direction);
  playr->projected_position.x +=
    shared_normalized_left_stick_direction.x *
    PLAYER_SPEED * mag * mag * delta_time;
  playr->projected_position.z +=
    shared_normalized_left_stick_direction.y *
    PLAYER_SPEED * mag * mag * delta_time;
  return 1;
}