#include <stdio.h>

#include "tail.h"

#include "constants.h"
#include "gpu_helpers.h"
#include "player.h"

#include "sphere_mesh.h"
#include "default_vert.h"
#include "solid_color_frag.h"
#include "normal_debug_frag.h"

#define PLAYER_SPEED 1.5f
#define DEADZONE 0.2f

static struct gpu_program idle_shader;
static struct gpu_program thrusting_shader;

static struct m4x4 shared_local_to_world;
static struct m3x3 shared_normals_local_to_world;

static struct vec2 shared_normalized_left_stick_direction;

void player__copy_assets_to_gpu(struct gpu_api const *const gpu) {
  idle_shader.frag_shader_src = solid_color_frag_src;
  idle_shader.vert_shader_src = default_vert_src;
  gpu->copy_program_to_gpu(&idle_shader);
  thrusting_shader.frag_shader_src = normal_debug_frag_src;
  thrusting_shader.vert_shader_src = default_vert_src;
  gpu->copy_program_to_gpu(&thrusting_shader);
  gpu->copy_static_mesh_to_gpu(&sphere_mesh);
}

/*
  IDLE
*/

static void player_idle__update(
    double delta_time,
    struct gamepad_input const *const input,
    struct player_state **const states,
    struct player *const playr
) {
  vec2__normalize(
    &input->left_stick_direction,
    &shared_normalized_left_stick_direction
  );
  float mag = vec2__magnitude(&input->left_stick_direction);
  if (mag > DEADZONE) {
    playr->current_state = states[PLAYER_STATE__THRUSTING];
    playr->transform.position.x +=
      shared_normalized_left_stick_direction.x *
      PLAYER_SPEED * mag * delta_time;
    playr->transform.position.z +=
      shared_normalized_left_stick_direction.y *
      PLAYER_SPEED * mag * delta_time;;
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
  gpu->select_gpu_program(&idle_shader);
  gpu->set_fragment_shader_vec3(&idle_shader, "color", &COLOR_WHITE);
  gpu__set_mvp(
    &shared_local_to_world,
    &shared_normals_local_to_world,
    cam,
    &idle_shader,
    gpu
  );
  gpu->draw_mesh(&sphere_mesh);
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
  vec2__normalize(
    &input->left_stick_direction,
    &shared_normalized_left_stick_direction
  );
  float mag = vec2__magnitude(&input->left_stick_direction);
  if (mag > DEADZONE) {
    playr->transform.rotation_in_deg.y += 900 * delta_time;
    playr->transform.position.x +=
      shared_normalized_left_stick_direction.x *
      PLAYER_SPEED * mag * delta_time;
    playr->transform.position.z +=
      shared_normalized_left_stick_direction.y *
      PLAYER_SPEED * mag * delta_time;;
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
  gpu->select_gpu_program(&thrusting_shader);
  gpu__set_mvp(
    &shared_local_to_world,
    &shared_normals_local_to_world,
    cam,
    &thrusting_shader,
    gpu
  );
  gpu->draw_mesh(&sphere_mesh);
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