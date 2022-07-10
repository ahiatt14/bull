#include <stdio.h> // TODO: remove
#include <math.h>

#include "tail.h"

#include "scene.h"
#include "constants.h"
#include "gpu_helpers.h"
#include "bull_math.h"
#include "player.h"

#include "sphere_mesh.h"

#include "solid_color_frag.h"
#include "normal_debug_frag.h"
#include "default_vert.h"

#define ARENA_RADIUS 2

#define PLAYER_STATE_COUNT 2

static double seconds_since_creation;
static double tick_start_time;
static double delta_time;

static double seconds_since_creation;

static struct camera cam;

static struct player_state *player_states[PLAYER_STATE_COUNT];
static struct player playr;
static struct gamepad_input input;

void arena__init(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu
) {

  camera__init(&cam);
  camera__set_position(0, 8, 0.1f, &cam); // TODO: not working if z is 0?
  camera__set_look_target(&ORIGIN, &cam);
  camera__set_horizontal_fov_in_deg(70, &cam);
  camera__set_near_clip_distance(1, &cam);
  camera__set_far_clip_distance(30, &cam);
  camera__calculate_lookat(&WORLDSPACE.up, &cam);
  camera__calculate_perspective(vwprt, &cam);

  player__copy_assets_to_gpu(gpu);
  player__init_states(player_states);
  playr = (struct player){
    .transform = (struct transform){{0,0,0},{0,0,0},0.33f},
    .current_state = player_states[PLAYER_STATE__IDLE]
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
  playr.current_state->update(
    delta_time,
    &input,
    player_states,
    &playr
  );
  // TODO: this should only run when player is moving!
  // face_player(&playr.transform);

  // DRAW
  gpu->clear(&COLOR_RED);

  playr.current_state->draw(&cam, gpu, &playr);
}