#include <stdio.h> // TODO: remove
#include <math.h>

#include "tail.h"

#include "constants.h"
#include "scene.h"
#include "bull_math.h"

#include "player.h"
#include "core.h"
#include "bouncers.h"

#include "solid_color_frag.h"
#include "normal_debug_frag.h"
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

static struct core_state core = (struct core_state){
  .transform = {
    {0,0,0},
    {0,0,0},
    CORE_RADIUS * 2
  }
};

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

  cam.position = (struct vec3){ 0, 40, 0.01f };
  cam.look_target = ORIGIN;
  cam.horizontal_fov_in_deg = 30;
  cam.near_clip_distance = 1;
  cam.far_clip_distance = 100;
  camera__calculate_lookat(WORLDSPACE.up, &cam);
  camera__calculate_perspective(vwprt, &cam);

  bouncers__copy_assets_to_gpu(gpu);
  core__copy_assets_to_gpu(gpu);

  player__copy_assets_to_gpu(gpu);

  ocean__init(
    window,
    vwprt,
    gpu
  );

  for (int i = 0; i < 36; i++) {
    bouncers__add_to_grid(3, i, &bouncy_grid);
    bouncers__add_to_grid(4, i, &bouncy_grid);
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

  bouncers__rotate_grid_row(
    3,
    30,
    delta_time,
    &bouncy_grid
  );
  bouncers__rotate_grid_row(
    4,
    -40,
    delta_time,
    &bouncy_grid
  );
  bouncers__radiate_grid(
    0.1f,
    delta_time,
    &bouncy_grid
  );

  player_one.previous_position = player_one.transform.position;

  player__update(
    delta_time,
    gamepad,
    &player_one_actions,
    &player_one
  );

  // static uint8_t player_one_is_outside_arena;
  // if (player_one_is_outside_arena) {
  //   player_one.transform.rotation_in_deg.y =
  //     rad_to_deg(atan(
  //       -player_one.transform.position.z /
  //       player_one.transform.position.x
  //     )) + 90;
  // } else {

  player_one.transform.rotation_in_deg.y =
    find_cw_or_ccw_facing_around_world_up(
      player_one.transform.position,
      player_one.previous_position
    );
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

  core__draw(&cam, gpu, &core);
  bouncers__draw_grid(&cam, gpu, &bouncy_grid);

  player__draw(&cam, gpu, &player_one);
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