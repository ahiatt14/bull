#include <stdio.h>
#include <math.h>

#include "tail.h"

#include "player.h"
#include "bull_math.h"
#include "constants.h"
#include "gpu_helpers.h"

#include "bird_mesh.h"
#include "default_vert.h"

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

// static struct shader shared_healthy_shader;

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

  if (gamepad.right_trigger >= TRIGGER_DEADZONE) {
    actions->start_auto_fire();
    playr->autofire_start_locked_to_cw = is_moving_cw_around_world_up(
      playr->projected_position,
      playr->transform.position
    );
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
  )) {
    playr->input_state = PLAYER_INPUT_STATE__IDLE;
    // printf("leaving thrusting\n");
    // printf(
    //   "pos x: %.5f z: %.5f -- prev: x: %.5f z: %.5f\n",
    //   playr->transform.position.x,
    //   playr->transform.position.z,
    //   playr->previous_position.x,
    //   playr->previous_position.z
    // );
  }

  if (gamepad.right_trigger >= TRIGGER_DEADZONE) {
    actions->start_auto_fire();
    playr->autofire_start_locked_to_cw = is_moving_cw_around_world_up(
      playr->projected_position,
      playr->transform.position
    );
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

  if (gamepad.right_trigger < TRIGGER_DEADZONE) {
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
  static struct m4x4 local_to_world;
  static struct m3x3 normals_local_to_world;

  space__create_model(
    &WORLDSPACE,
    &playr->transform,
    &local_to_world
  );
  space__create_normals_model(
    &local_to_world,
    &normals_local_to_world
  );
  gpu->select_shader(&SOLID_COLOR_SHADER);
  gpu->set_fragment_shader_vec3(
    &SOLID_COLOR_SHADER,
    "color",
    COLOR_WHITE
  );
  gpu__set_mvp(
    &local_to_world,
    &normals_local_to_world,
    cam,
    &SOLID_COLOR_SHADER,
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
  static float mag;
  static struct vec2 normalized_left_stick_direction;

  mag = vec2__magnitude(direction);
  if (mag < STICK_DEADZONE) return 0;

  playr->previous_position = playr->transform.position;
  
  normalized_left_stick_direction = vec2__normalize(direction);
  playr->projected_position.x +=
    normalized_left_stick_direction.x *
    PLAYER_SPEED * mag * mag * delta_time;
  playr->projected_position.z +=
    normalized_left_stick_direction.y *
    PLAYER_SPEED * mag * mag * delta_time;
  return 1;
}