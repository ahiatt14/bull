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

#define LEFT 1
#define RIGHT 0

static struct shader idle_shader;
static struct shader thrusting_shader;

static struct m4x4 shared_local_to_world;
static struct m3x3 shared_normals_local_to_world;

static struct vec2 shared_normalized_left_stick_direction;

static uint8_t player_is_moving_cw(
  struct player const *const playr
);
static void face_player(struct player *const playr);
static void move_player(
  struct player *const playr,
  struct vec2 const *const direction,
  double delta_time
);
static void save_player_history(struct player *const playr);

/*
  IDLE
*/

static void player_idle__update(
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
    face_player(playr);
    playr->current_state = PLAYER_STATE__THRUSTING;
  } else {

  }
  save_player_history(playr);
}

static void player_idle__draw(
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
  gpu->select_shader(&idle_shader);
  gpu->set_fragment_shader_vec3(&idle_shader, "color", &COLOR_WHITE);
  gpu__set_mvp(
    &shared_local_to_world,
    &shared_normals_local_to_world,
    cam,
    &idle_shader,
    gpu
  );
  gpu->draw_mesh(&bird_mesh);
}

/*
  THRUSTING
*/

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
    // if (
    //   player_is_moving_cw(playr) &&
    //   playr.moved_cw_last_frame
    // )
    face_player(playr);
  } else {
    playr->current_state = PLAYER_STATE__IDLE;
  }
  save_player_history(playr);
}

static void player_thrusting__draw(
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
  gpu->select_shader(&thrusting_shader);
  gpu__set_mvp(
    &shared_local_to_world,
    &shared_normals_local_to_world,
    cam,
    &thrusting_shader,
    gpu
  );
  gpu->draw_mesh(&bird_mesh);
}

static void player_cw_to_ccw__udpate(
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
  }
  save_player_history(playr);
}

// INITIALIZATION

// TODO: no reason not to share some draw fns here!
static struct player_state idle_state = {
  .update = player_idle__update,
  .draw = player_idle__draw,
};
static struct player_state thrusting_state = {
  .update = player_thrusting__update,
  .draw = player_thrusting__draw
};
// static struct player_state cw_to_ccw_state = {
//   .update = player_cw_to_ccw__udpate,
//   .draw = player_cw_to_ccw__draw
// };

void player__init_states(struct player_state **const states) {
  states[PLAYER_STATE__IDLE] = &idle_state;
  states[PLAYER_STATE__THRUSTING] = &thrusting_state;
}

void player__copy_assets_to_gpu(struct gpu_api const *const gpu) {
  idle_shader.frag_shader_src = solid_color_frag_src;
  idle_shader.vert_shader_src = default_vert_src;
  gpu->copy_shader_to_gpu(&idle_shader);
  thrusting_shader.frag_shader_src = solid_color_frag_src;
  thrusting_shader.vert_shader_src = default_vert_src;
  gpu->copy_shader_to_gpu(&thrusting_shader);
  gpu->copy_static_mesh_to_gpu(&bird_mesh);
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

static uint8_t player_is_moving_cw(
  struct player const *const playr
) {
  struct vec3 previous_to_current_position = {0};
  struct vec3 cross = {0};
  vec3_minus_vec3(
    &playr->transform.position,
    &playr->previous_position,
    &previous_to_current_position
  );
  vec3__cross(
    &playr->transform.position,
    &previous_to_current_position,
    &cross
  );
  return cross.y <= 0 ? 1 : 0;
}

static void face_player(
  struct player *const playr
) {
  uint8_t moving_cw = player_is_moving_cw(playr);
  float ccw_rotation_in_deg = rad_to_deg(atan(
    -playr->transform.position.z /
    playr->transform.position.x
  ));
  if (playr->transform.position.x < 0) ccw_rotation_in_deg += 180;
  playr->transform.rotation_in_deg.y =
    moving_cw ?
    ccw_rotation_in_deg + 180 :
    ccw_rotation_in_deg;
}

static void save_player_history(
  struct player *const playr
) {
  playr->previously_moving_cw = player_is_moving_cw(playr);
  playr->previous_position = playr->transform.position;
}