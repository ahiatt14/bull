#include <stdio.h> // TODO: remove
#include <math.h>

#include "tail.h"

#include "constants.h"
#include "scene.h"
#include "bull_math.h"

#include "player.h"
#include "arena.h"
#include "core.h"

#include "solid_color_frag.h"
#include "normal_debug_frag.h"
#include "default_vert.h"

// CONSTANTS

#define CORE_RADIUS 0.66f
#define ARENA_RADIUS 3

#define PLAYER_INPUT_STATE_COUNT 2
#define PLAYER_EFFECT_STATE_COUNT 1

// FORWARD DECS

static uint8_t player_is_moving_cw(struct player const *const playr);
static void face_player(struct player *const playr);


// LOCALS

static double seconds_since_creation;
static double tick_start_time;
static double delta_time;

static struct camera cam;
static struct gamepad_input gamepad;

static struct arena_state arena = (struct arena_state){
  .transform = {
    {0,0,0},
    {0,0,0},
    ARENA_RADIUS * 2
  }
};

static struct core_state core = (struct core_state){
  .transform = {
    {0,0,0},
    {0,0,0},
    CORE_RADIUS * 2
  }
};

struct player player_one = (struct player){
  .transform = {{1, 0, 0}, {0, 0, 0}, 1},
  .input_state = PLAYER_INPUT_STATE__IDLE,
  .effect_state = PLAYER_EFFECT_STATE__HEALTHY
};

// TODO: move these lists back into player.c!
void (*player_input_state_updates[PLAYER_INPUT_STATE_COUNT])(
  double delta_time,
  struct gamepad_input const *const gamepad,
  struct player *const player_one,
  struct player_actions const *const actions 
);

void (*player_effect_state_updates[PLAYER_EFFECT_STATE_COUNT])(
  double delta_time,
  struct player *const player_one
);

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

  camera__init(&cam);
  camera__set_position(0, 20, 1, &cam); // TODO: not working if z is 0?
  camera__set_look_target(&ORIGIN, &cam);
  camera__set_horizontal_fov_in_deg(50, &cam);
  camera__set_near_clip_distance(1, &cam);
  camera__set_far_clip_distance(50, &cam);
  camera__calculate_lookat(&WORLDSPACE.up, &cam);
  camera__calculate_perspective(vwprt, &cam);

  core__copy_assets_to_gpu(gpu);
  arena__copy_assets_to_gpu(gpu);

  // TODO: already forgot why we want the state update lists
  // out here instead of hidden in the player module smdh
  player__init_state_lists(
    player_input_state_updates,
    player_effect_state_updates
  );
  player__copy_assets_to_gpu(gpu);
}

void action__tick(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu,
  uint8_t previous_scene,
  void switch_scene(uint8_t new_scene)
) {

  // GAME TIME

  seconds_since_creation = window->get_seconds_since_creation();
  delta_time = seconds_since_creation - tick_start_time;
  if (delta_time > DELTA_TIME_CAP) delta_time = DELTA_TIME_CAP;
  tick_start_time = seconds_since_creation;

  // HANDLE UNPLUGGED GAMEPAD
  
  if (!window->gamepad_is_connected()) {
    switch_scene(SCENE__CONNECT_GAMEPAD);
    return;
  }

  // GAMEPLAY
  
  window->get_gamepad_input(gamepad);
  player_input_state_updates[player_one.input_state](
    delta_time,
    gamepad,
    &player_one,
    &player_one_actions
  );
  player_one.transform.position = player_one.desired_position;

  // DRAW

  gpu->clear(&COLOR_BLACK);

  arena__draw(&cam, gpu, &arena);
  core__draw(&cam, gpu, &core);
  player__draw(&cam, gpu, &player_one);
}

// HELPERS

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