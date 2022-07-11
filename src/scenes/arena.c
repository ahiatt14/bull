#include <stdio.h> // TODO: remove
#include <math.h>

#include "tail.h"

#include "scene.h"
#include "constants.h"
#include "gpu_helpers.h"
#include "bull_math.h"
#include "player.h"

#include "sphere_mesh.h"
#include "core_mesh.h"
#include "cage_mesh.h"

#include "arena_cage_frag.h"
#include "solid_color_frag.h"
#include "normal_debug_frag.h"
#include "default_vert.h"

#include "noise_texture.h"
#include "noise2_texture.h"

#define ARENA_RADIUS 3
#define CORE_RADIUS 1

#define PLAYER_STATE_COUNT 2

static double seconds_since_creation;
static double tick_start_time;
static double delta_time;

static double seconds_since_creation;

static struct camera cam;

static struct transform cage_transform =
  (struct transform){
    {0,0,0},
    {0,0,0},
    ARENA_RADIUS * 2
  };
static struct shader cage_shader;

// static struct transform core_transform =
//   (struct transform){
//     {0,0,0},
//     {0,0,0},
//     CORE_RADIUS * 2
//   };
static struct shader core_shader;
static struct m4x4 shared_local_to_world;
static struct m3x3 shared_normals_local_to_world;

static struct player_state *player_states[PLAYER_STATE_COUNT];
static struct player playr;
static struct gamepad_input input;

void arena__init(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu
) {

  camera__init(&cam);
  camera__set_position(0, 15, 1, &cam); // TODO: not working if z is 0?
  camera__set_look_target(&ORIGIN, &cam);
  camera__set_horizontal_fov_in_deg(50, &cam);
  camera__set_near_clip_distance(1, &cam);
  camera__set_far_clip_distance(50, &cam);
  camera__calculate_lookat(&WORLDSPACE.up, &cam);
  camera__calculate_perspective(vwprt, &cam);

  gpu->copy_rgb_texture_to_gpu(&noise_texture);
  gpu->copy_rgb_texture_to_gpu(&noise2_texture);

  gpu->copy_static_mesh_to_gpu(&core_mesh);
  gpu->copy_static_mesh_to_gpu(&cage_mesh);

  core_shader.frag_shader_src = solid_color_frag_src;
  core_shader.vert_shader_src = default_vert_src;
  gpu->copy_shader_to_gpu(&core_shader);

  cage_shader.frag_shader_src = arena_cage_frag_src;
  cage_shader.vert_shader_src = default_vert_src;
  gpu->copy_shader_to_gpu(&cage_shader);

  player__copy_assets_to_gpu(gpu);
  player__init_states(player_states);
  playr = (struct player){
    .transform = (struct transform){{0,0,0},{0,0,0},0.33f},
    .previous_position = (struct vec3){0,0,0},
    .current_state = PLAYER_STATE__IDLE
  };
}

void arena__tick(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu,
  uint8_t previous_scene,
  void switch_scene(uint8_t new_scene)
) {
  seconds_since_creation = window->get_seconds_since_creation();
  delta_time = seconds_since_creation - tick_start_time;
  if (delta_time > DELTA_TIME_CAP) delta_time = DELTA_TIME_CAP;
  tick_start_time = seconds_since_creation;

  if (!window->gamepad_is_connected()) {
    switch_scene(SCENE__CONNECT_GAMEPAD);
    return;
  }
  
  window->get_gamepad_input(&input);
  player_states[playr.current_state]->update(
    delta_time,
    &input,
    &playr
  );

  cage_transform.rotation_in_deg.x += 3 * delta_time;
  cage_transform.rotation_in_deg.z += 7 * delta_time;

  // DRAW
  gpu->clear(&COLOR_BLACK);

  player_states[playr.current_state]->draw(&cam, gpu, &playr);

  gpu->cull_back_faces();
  gpu->select_shader(&cage_shader);
  gpu->select_textures(
    // TODO: cache an array for this
    (struct texture const *const []){ &noise_texture, &noise2_texture },
    2
  );
  space__create_model(
    &WORLDSPACE,
    &cage_transform,
    &shared_local_to_world
  );
  space__create_normals_model(
    &shared_local_to_world,
    &shared_normals_local_to_world
  );
  gpu__set_mvp(
    &shared_local_to_world,
    &shared_normals_local_to_world,
    &cam,
    &cage_shader,
    gpu
  );
  gpu->draw_wireframe(&cage_mesh);
}