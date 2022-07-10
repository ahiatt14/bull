#include <stdio.h>
#include <math.h>

#include "tail.h"

#include "bull_math.h"
#include "constants.h"
#include "gpu_helpers.h"
#include "player.h"

#include "ship_mesh.h"
#include "default_vert.h"
#include "solid_color_frag.h"
#include "normal_debug_frag.h"

#define PLAYER_SPEED 3.0f
#define DEADZONE 0.2f

#define LEFT 1
#define RIGHT 0

static struct shader idle_shader;
static struct shader thrusting_shader;

static struct m4x4 shared_local_to_world;
static struct m3x3 shared_normals_local_to_world;

static struct vec2 shared_normalized_left_stick_direction;

static void face_player(struct player *const playr);
static void move_player(
  struct transform *const position,
  struct vec2 const *const direction,
  double delta_time
);

/*
  IDLE
*/

static void player_idle__update(
    double delta_time,
    struct gamepad_input const *const input,
    struct player_state **const states,
    struct player *const playr
) {
  if (vec2__magnitude(&input->left_stick_direction) > DEADZONE) {
    move_player(
      &playr->transform,
      &input->left_stick_direction,
      delta_time
    );
    face_player(playr);
    playr->current_state = states[PLAYER_STATE__THRUSTING];
  }
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
  gpu->draw_mesh(&ship_mesh);
}

/*
  THRUSTING
*/

static void player_thrusting__update(
    double delta_time,
    struct gamepad_input const *const input,
    struct player_state **const states,
    struct player *const playr
) {
  if (vec2__magnitude(&input->left_stick_direction) > DEADZONE) {
    move_player(
      &playr->transform,
      &input->left_stick_direction,
      delta_time
    );
    face_player(playr);
  } else {
    playr->current_state = states[PLAYER_STATE__IDLE];
  }
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
  gpu->draw_mesh(&ship_mesh);
}

static struct player_state idle_state = {
  .update = player_idle__update,
  .draw = player_idle__draw,
};
static struct player_state thrusting_state = {
  .update = player_thrusting__update,
  .draw = player_thrusting__draw
};

void player__init_states(struct player_state **const states) {
  states[PLAYER_STATE__IDLE] = &idle_state;
  states[PLAYER_STATE__THRUSTING] = &thrusting_state;
}





// HELPERS

static void move_player(
  struct transform *const position,
  struct vec2 const *const direction,
  double delta_time
) {
  float mag = vec2__magnitude(direction);
  vec2__normalize(
    direction,
    &shared_normalized_left_stick_direction
  );
  position->position.x +=
    shared_normalized_left_stick_direction.x *
    PLAYER_SPEED * mag * mag * delta_time;
  position->position.z +=
    shared_normalized_left_stick_direction.y *
    PLAYER_SPEED * mag * mag * delta_time;
}

void player__copy_assets_to_gpu(struct gpu_api const *const gpu) {
  idle_shader.frag_shader_src = solid_color_frag_src;
  idle_shader.vert_shader_src = default_vert_src;
  gpu->copy_shader_to_gpu(&idle_shader);
  thrusting_shader.frag_shader_src = normal_debug_frag_src;
  thrusting_shader.vert_shader_src = default_vert_src;
  gpu->copy_shader_to_gpu(&thrusting_shader);
  gpu->copy_static_mesh_to_gpu(&ship_mesh);
}

static void face_player(
  struct player *const playr
) {
  struct vec3 center_to_player = {0};
  struct vec3 previous_to_current_position = {0};
  struct vec3 cross = {0};
  vec3_minus_vec3(
    &playr->transform.position,
    &ORIGIN,
    &center_to_player
  );
  vec3_minus_vec3(
    &playr->transform.position,
    &playr->previous_position,
    &previous_to_current_position
  );
  vec3__cross(
    &center_to_player,
    &previous_to_current_position,
    &cross
  );
  // TODO: could clean/reduce redudancy here
  struct vec2 facing = cross.z ?
    vec2__turn_90_deg(
      LEFT,
      (struct vec2){ center_to_player.x, center_to_player.z }
    ) :
    vec2__turn_90_deg(
      RIGHT,
      (struct vec2){ center_to_player.x, center_to_player.z }
    );

  printf(
    "c2p: %.2f %.2f pr2p: %.8f %.8f fcng: %.2f %.2f\n",
    center_to_player.x, center_to_player.z,
    previous_to_current_position.x, previous_to_current_position.z,
    facing.x, facing.y
  );

  playr->transform.rotation_in_deg.y =
    rad_to_deg(atan2(facing.y, facing.x));

  playr->previous_position = playr->transform.position;
}