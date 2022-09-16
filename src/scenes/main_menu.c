#include <stdio.h>

#include "tail.h"

#include "tail_helpers.h"
#include "scene.h"
#include "constants.h"

#include "pyramid_mesh.h"
#include "smooth_cube_mesh.h"

#include "default_vert.h"

#define SEC_UNTIL_ACTION 2.0f

void main_menu__init(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu
) {

}

void main_menu__tick(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu,
  uint8_t previous_scene,
  void (*switch_scene)(uint8_t new_scene)
) {
  static double seconds_since_creation;
  static double tick_start_time;
  static double delta_time;

  tick_start_time = seconds_since_creation;
  seconds_since_creation = window->get_seconds_since_creation();
  delta_time = seconds_since_creation - tick_start_time;
  if (delta_time > DELTA_TIME_CAP) delta_time = DELTA_TIME_CAP;

  // if (!window->gamepad_is_connected()) {
  //   switch_scene(SCENE__CONNECT_GAMEPAD);
  //   return;
  // }

  // UPDATE

  static double sec_until_action = SEC_UNTIL_ACTION;
  sec_until_action -= delta_time;
  if (sec_until_action <= 0) {
    sec_until_action = SEC_UNTIL_ACTION;
    switch_scene(SCENE__ACTION);
    return;
  }

  // DRAW

  gpu->cull_back_faces();

  gpu->select_shader(&SOLID_COLOR_SHADER);
  gpu->set_shader_vec3(
    &SOLID_COLOR_SHADER,
    "color",
    COLOR_DARK_GREY_BLUE
  );
  gpu->draw_mesh(&QUAD);
}