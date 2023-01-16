#include <stdio.h>
#include <math.h>

#include "tail.h"

#include "scene.h"

#include "constants.h"
#include "bull_math.h"
#include "tail_helpers.h"

#include "player.h"
#include "fireballs.h"
#include "bouncers.h"
#include "firing_guide.h"

/*
  ~~~~~~~~~CONSTANTS~~~~~~~~~~
*/

#define PLAYER_LVL_COUNT 3
#define PLAYER_START_POS {3, 0, 0}

#define GUIDE_LAG_TIME_SECONDS 0.15f

#define ARENA_EDGE_RADIUS 8.0f

typedef void (*player_one_autofire_ptr)(
  struct gametime time,
  struct player const *const playr
);

struct guide_lag_state {
  double seconds_since_player_moved;
  struct vec3 guide_target_position;
};

/*
  ~~~~~~~~~FORWARD DECS~~~~~~~~~~
*/

static void bounce_player(
  uint8_t row,
  uint8_t column,
  struct bouncer_grid *const grid
);

static void begin_lvl0_fireball_autofire(
  struct gametime time,
  struct player const *const playr
);

static void autofire_lvl0_fireballs(
  struct gametime time,
  struct player const *const playr
);

static void stop_autofiring(
  struct gametime time,
  struct player const *const playr
);

void guide_lag_update(
  struct gametime time,
  struct player const *const playr,
  struct guide_lag_state *const guide_lag
);

/*
  ~~~~~~~~~LOCAL STATE~~~~~~~~~~
*/

static struct camera cam;
static struct gamepad_input gamepad;

static struct bouncer_grid bouncy_grid;

static struct player player_one = {
  .transform = {PLAYER_START_POS, {0, 0, 0}, 1},
  .previous_position = PLAYER_START_POS,
  .projected_position = PLAYER_START_POS,
  .input_state = PLAYER_INPUT_STATE__IDLE,
  .effect_state = PLAYER_EFFECT_STATE__HEALTHY
};
player_one_autofire_ptr player_one_autofire = NULL;
static double seconds_until_next_autofire_shot;

static struct player_actions player_one_actions ={
  .start_autofire = begin_lvl0_fireball_autofire,
  .stop_autofire = stop_autofiring
};

static struct guide_lag_state guide_lag = {
  .guide_target_position = PLAYER_START_POS
};

static struct fireballs fbs;

/*
  ~~~~~~~~~PUBLIC API~~~~~~~~~~
*/

void action__init(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu
) {

  cam.position = (struct vec3){ 0, 20, 8 };
  cam.look_target = (struct vec3){
    ORIGIN.x,
    ORIGIN.y,
    ORIGIN.z + 1.2f
  };
  cam.horizontal_fov_in_deg = 58;
  cam.near_clip_distance = 0.3f;
  cam.far_clip_distance = 100;

  camera__calculate_lookat(WORLDSPACE.up, &cam);
  camera__calculate_perspective(vwprt, &cam);
  // camera__calculate_ortho(12, 9, -4, 4, &cam);

  bouncers__copy_assets_to_gpu(gpu);
  fireballs__copy_assets_to_gpu(gpu);

  player__copy_assets_to_gpu(gpu);
  firing_guide__copy_assets_to_gpu(gpu);

  ocean__init(window, vwprt, gpu);

  for (int i = 0; i < BOUNCER_GRID_MAX_PER_ROW; i++) {
    if (i < BOUNCER_GRID_MAX_PER_ROW / 2) {
      bouncers__add_to_grid(4, i, &bouncy_grid);
    } else {
      bouncers__add_to_grid(6, i, &bouncy_grid);
    }
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
  guide_lag_update(time, &player_one, &guide_lag);

  if (player_one_autofire) player_one_autofire(
    time,
    &player_one
  );

  fireballs__move(
    time,
    25,
    ARENA_EDGE_RADIUS,
    &fbs
  );

  bouncers__rotate_grid_row(4, 10, time, &bouncy_grid);
  bouncers__rotate_grid_row(6, -15, time, &bouncy_grid);
  // bouncers__radiate_grid(0.1f, time, &bouncy_grid);

  bouncers__check_collision_with_grid(
    bounce_player,
    player_one.projected_position,
    &bouncy_grid
  );

  // TODO: abstract
  static int deg_mod = 0;
  deg_mod = player_one.transform.position.x >= 0 ? 90 : -90;
  player_one.transform.rotation_in_deg.y =
    rad_to_deg(atan(
      -player_one.transform.position.z /
      player_one.transform.position.x
    )) + deg_mod;
  
  // if (
  //   vec3__distance(player_one.projected_position, ORIGIN) >=
  //   ARENA_EDGE_RADIUS
  // ) player_one.projected_position =
  //   slide_along_radius_around_world_origin(
  //     ARENA_EDGE_RADIUS,
  //     player_one.projected_position,
  //     player_one.transform.position
  //   );

  player_one.transform.position = player_one.projected_position;

  // DRAW

  ocean__tick(time, window, vwprt, gpu, SCENE__MAIN_MENU, NULL);
  gpu->clear_depth_buffer();

  bouncers__draw_grid(time, &cam, gpu, &bouncy_grid);

  fireballs__draw(&cam, gpu, &fbs);

  player__draw(&cam, gpu, &player_one);
  firing_guide__draw(
    &cam,
    gpu,
    ARENA_EDGE_RADIUS,
    guide_lag.guide_target_position
  );

}

/*
  ~~~~~~~~ FUNCTION DEFINITIONS ~~~~~~~~~
*/

// static float 

static void bounce_player(
  uint8_t row,
  uint8_t column,
  struct bouncer_grid *const grid
) {

  static struct battlefield_pos guide_target_bfpos;
  guide_target_bfpos = world_to_battlefield_pos(guide_lag.guide_target_position);
  guide_target_bfpos.degrees = (int)(guide_target_bfpos.degrees + 180) % 360;

  guide_lag.guide_target_position = battlefield_to_world_pos(guide_target_bfpos);
  guide_lag.seconds_since_player_moved = GUIDE_LAG_TIME_SECONDS;

  player_one.input_state = PLAYER_INPUT_STATE__FLIPPING;
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
  seconds_until_next_autofire_shot = 0;
  player_one_autofire = NULL;
}

static void autofire_lvl0_fireballs(
  struct gametime time,
  struct player const *const playr
) {

  seconds_until_next_autofire_shot -= time.delta;
  if (seconds_until_next_autofire_shot > 0) return;
  seconds_until_next_autofire_shot = 0.15f;
    // FIREBALL_SHOT_SEC_INTERVAL_BY_LVL[playr->level];

  fireballs__activate(
    world_to_battlefield_pos(playr->transform.position),
    &fbs
  );
}

void guide_lag_update(
  struct gametime time,
  struct player const *const playr,
  struct guide_lag_state *const guide_lag
) {
  if (vec3__distance(
    playr->previous_position,
    playr->transform.position
  ) <= 0.02) guide_lag->seconds_since_player_moved = 0;
  guide_lag->seconds_since_player_moved += time.delta;
  if (guide_lag->seconds_since_player_moved > GUIDE_LAG_TIME_SECONDS)
    guide_lag->seconds_since_player_moved = GUIDE_LAG_TIME_SECONDS;
  guide_lag->guide_target_position = vec3__lerp(
    guide_lag->guide_target_position,
    playr->transform.position,
    guide_lag->seconds_since_player_moved / GUIDE_LAG_TIME_SECONDS
  );
}