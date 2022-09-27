#include <stdio.h> // TODO: remove
#include <math.h>

#include "tail.h"

#include "scene.h"

#include "constants.h"
#include "bull_math.h"
#include "tail_helpers.h"

#include "player.h"
#include "fireballs.h"
#include "core.h"
#include "bouncers.h"
#include "firing_guide.h"

/*
  ~~~~~~~~~CONSTANTS~~~~~~~~~~
*/

#define CORE_RADIUS 1

#define PLAYER_START_POS {3, 0, 0}

#define MIN_PLAYER_RADIUS CORE_RADIUS * 2.1f

typedef void (*player_one_autofire_ptr)(
  struct gametime time,
  struct player const *const playr
);

/*
  ~~~~~~~~~FORWARD DECS~~~~~~~~~~
*/

static void bounce_player(
  uint8_t row,
  uint8_t column,
  struct vec3 bouncer_to_target,
  struct bouncer_grid *const grid
);

static void stop_autofiring(
  struct gametime time,
  struct player const *const playr
);

static void begin_lvl0_fireball_autofire(
  struct gametime time,
  struct player const *const playr
);

static void autofire_lvl0_fireballs(
  struct gametime time,
  struct player const *const playr
);

/*
  ~~~~~~~~~LOCAL STATE~~~~~~~~~~
*/

static struct camera cam;
static struct gamepad_input gamepad;

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
player_one_autofire_ptr player_one_autofire = NULL;

static struct player_actions player_one_actions = (struct player_actions){
  .start_autofire = begin_lvl0_fireball_autofire,
  .stop_autofire = stop_autofiring
};

/*
  ~~~~~~~~~PUBLIC API~~~~~~~~~~
*/

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

  // core__copy_assets_to_gpu(gpu);

  player__copy_assets_to_gpu(gpu);
  firing_guide__copy_assets_to_gpu(gpu);

  ocean__init(window, vwprt, gpu);

  for (int i = 0; i < BOUNCER_GRID_MAX_PER_ROW; i++) {
    // bouncers__add_to_grid(4, i, &bouncy_grid);
    bouncers__add_to_grid(6, i, &bouncy_grid);
    bouncers__add_to_grid(8, i, &bouncy_grid);
  }
}

void action__tick(
  struct gametime time,
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu,
  uint8_t previous_scene,
  void switch_scene(uint8_t new_scene)
) {

  window->get_gamepad_input(&gamepad);

  // HANDLE UNPLUGGED GAMEPAD
  
  // if (!window->gamepad_is_connected()) {
  //   switch_scene(SCENE__CONNECT_GAMEPAD);
  //   return;
  // }

  // GAMEPLAY

  player__update(time, gamepad, &player_one_actions, &player_one);

  if (player_one_autofire) player_one_autofire(
    time,
    &player_one
  );

  bouncers__rotate_grid_row(4, 10, time, &bouncy_grid);
  bouncers__rotate_grid_row(6, -15, time, &bouncy_grid);
  bouncers__rotate_grid_row(8, 20, time, &bouncy_grid);
  bouncers__radiate_grid(0.1f, time, &bouncy_grid);

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

  ocean__tick(time, window, vwprt, gpu, SCENE__MAIN_MENU, NULL);
  gpu->clear_depth_buffer();

  // core__draw(&cam, gpu, &core);
  bouncers__draw_grid(time, &cam, gpu, &bouncy_grid);

  player__draw(&cam, gpu, &player_one);
  firing_guide__draw(&cam, gpu, player_one.transform.position);
}

/*
  ~~~~~~~~ FUNCTION DEFINITIONS ~~~~~~~~~
*/

static void bounce_player(
  uint8_t row,
  uint8_t column,
  struct vec3 bouncer_to_target,
  struct bouncer_grid *const grid
) {
  bouncers__delete_from_grid(row, column, grid);
}

static void begin_lvl0_fireball_autofire(
  struct gametime time,
  struct player const *const playr
) {
  player_one_autofire = autofire_lvl0_fireballs;
  player_one_autofire(time, playr);
}

static void stop_autofiring(
  struct gametime time,
  struct player const *const playr
) {
  player_one_autofire = NULL;
}

static void autofire_lvl0_fireballs(
  struct gametime time,
  struct player const *const playr
) {

}