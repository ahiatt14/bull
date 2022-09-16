#include <stdio.h> // TODO: remove
#include <math.h>

#include "tail.h"

#include "constants.h"
#include "scene.h"
#include "bull_math.h"
#include "tail_helpers.h"

#include "player.h"
#include "core.h"
#include "bouncers.h"

#include "firing_guide_frag.h"
#include "default_vert.h"

// CONSTANTS

#define CORE_RADIUS 1

#define PLAYER_START_POS {3, 0, 0}

#define MIN_PLAYER_RADIUS CORE_RADIUS * 2.1f

// FORWARD DECS

static void noop(uint8_t d) {}
struct vec3 slide_along_radius_around_world_origin(
  float radius,
  struct vec3 projected_pos,
  struct vec3 pos
);

// LOCALS

static struct camera cam;
static struct gamepad_input gamepad;

static struct shader guide_shader;
static struct m4x4 guide_local_to_world;
static struct m3x3 guide_normals_local_to_world;
static struct transform guide_transform = (struct transform){
  .position = {0,-0.1f,0},
  .rotation_in_deg = {-90,0,0},
  .scale = 11
};

// static struct core_state core = (struct core_state){
//   .transform = {
//     {0,0,0},
//     {0,0,0},
//     CORE_RADIUS * 2
//   }
// };

static struct bouncer_grid bouncy_grid;

struct player player_one = (struct player){
  .transform = {PLAYER_START_POS, {0, 0, 0}, 1},
  .previous_position = PLAYER_START_POS,
  .projected_position = PLAYER_START_POS,
  .input_state = PLAYER_INPUT_STATE__IDLE,
  .effect_state = PLAYER_EFFECT_STATE__HEALTHY
};

static void print_autofiring() {
  printf("autofiring!\n");
}

static void print_autofire_stop() {
  printf("autofire stopped\n");
}

static void bounce_player(
  uint8_t row,
  uint8_t column,
  struct vec3 bouncer_to_target,
  struct bouncer_grid *const grid
) {
  bouncers__delete_from_grid(row, column, grid);
}

struct player_actions player_one_actions = (struct player_actions){
  print_autofiring,
  print_autofire_stop
};

// PUBLIC API

void action__init(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu
) {

  cam.position = (struct vec3){ 0, 1, 0.1f };
  cam.look_target = ORIGIN;

  camera__calculate_lookat(WORLDSPACE.up, &cam);
  camera__calculate_ortho(12, 9, -4, 4, &cam);

  bouncers__copy_assets_to_gpu(gpu);
  core__copy_assets_to_gpu(gpu);

  player__copy_assets_to_gpu(gpu);

  guide_shader.frag_src = firing_guide_frag_src;
  guide_shader.vert_src = default_vert_src;
  gpu->copy_shader_to_gpu(&guide_shader);
  space__create_model(&WORLDSPACE, &guide_transform, &guide_local_to_world);
  space__create_normals_model(
    &guide_local_to_world,
    &guide_normals_local_to_world
  );

  ocean__init(window, vwprt, gpu);

  for (int i = 0; i < BOUNCER_GRID_MAX_PER_ROW; i++) {
    // bouncers__add_to_grid(4, i, &bouncy_grid);
    bouncers__add_to_grid(6, i, &bouncy_grid);
    bouncers__add_to_grid(8, i, &bouncy_grid);
  }
}

void action__tick(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu,
  uint8_t previous_scene,
  void switch_scene(uint8_t new_scene)
) {
  static double seconds_since_creation;
  static double tick_start_time;
  static double delta_time;

  // GAME TIME

  tick_start_time = seconds_since_creation;
  seconds_since_creation = window->get_seconds_since_creation();
  delta_time = seconds_since_creation - tick_start_time;
  if (delta_time > DELTA_TIME_CAP) delta_time = DELTA_TIME_CAP;

  window->get_gamepad_input(&gamepad);

  // HANDLE UNPLUGGED GAMEPAD
  
  // if (!window->gamepad_is_connected()) {
  //   switch_scene(SCENE__CONNECT_GAMEPAD);
  //   return;
  // }

  // GAMEPLAY

  player__update(
    delta_time,
    gamepad,
    &player_one_actions,
    &player_one
  );

  bouncers__rotate_grid_row(4, 10, delta_time, &bouncy_grid);
  bouncers__rotate_grid_row(6, -15, delta_time, &bouncy_grid);
  bouncers__rotate_grid_row(8, 20, delta_time, &bouncy_grid);
  bouncers__radiate_grid(0.1f, delta_time, &bouncy_grid);

  bouncers__check_collision_with_grid(
    bounce_player,
    player_one.projected_position,
    &bouncy_grid
  );

  player_one.transform.rotation_in_deg.y =
    find_cw_or_ccw_facing_around_world_up(
      player_one.projected_position,
      player_one.previous_position
    );

  // static uint8_t player_one_is_outside_arena;
  // if (player_one_is_outside_arena) {
  //   player_one.transform.rotation_in_deg.y =
  //     rad_to_deg(atan(
  //       -player_one.transform.position.z /
  //       player_one.transform.position.x
  //     )) + 90;
  // } else {
  // }
  
  // TODO: lock player facing when autofiring
  
  if (
    vec3__distance(player_one.projected_position, ORIGIN) <
    MIN_PLAYER_RADIUS
  ) player_one.projected_position = 
    slide_along_radius_around_world_origin(
      MIN_PLAYER_RADIUS,
      player_one.projected_position,
      player_one.transform.position
    );

  player_one.transform.position = player_one.projected_position;

  // DRAW

  ocean__tick(
    window,
    vwprt,
    gpu,
    SCENE__MAIN_MENU,
    noop
  );
  gpu->clear_depth_buffer();

  // core__draw(&cam, gpu, &core);
  bouncers__draw_grid(&cam, gpu, &bouncy_grid);

  player__draw(&cam, gpu, &player_one);

  gpu->select_shader(&guide_shader);
  gpu__set_mvp(
    &guide_local_to_world,
    &M3X3_IDENTITY,
    &cam,
    &guide_shader,
    gpu
  );
  gpu->set_shader_vec3(
    &guide_shader,
    "color",
    COLOR_RED
  );
  gpu->set_shader_float(
    &guide_shader,
    "player_radius_world",
    vec3__distance(
      player_one.transform.position,
      ORIGIN
    )
  );
  gpu->draw_mesh(&QUAD);
}

// HELPERS

struct vec3 slide_along_radius_around_world_origin(
  float radius,
  struct vec3 projected_pos,
  struct vec3 pos
) {
  struct vec3 pos_at_radius = scalar_x_vec3(
    radius / vec3__distance(pos, ORIGIN),
    pos
  );
  float distance_left = vec3__distance(projected_pos, pos_at_radius);
  float rads = rads_from_arc_len_and_radius(distance_left, radius);
  if (is_moving_cw_around_world_up(projected_pos, pos)) rads = -rads;
  struct m4x4 rotation = {0};
  m4x4__rotation(rads, WORLDSPACE.up, &rotation);
  return m4x4_x_point(&rotation, pos_at_radius);
}