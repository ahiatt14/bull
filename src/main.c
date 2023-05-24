#include <stdio.h>
#include <stdint.h>

#include "tail.h"

#include "colors.h"
#include "assets.h"
#include "constants.h"
#include "scene.h"

#include "normal_debug_geo.h"
#include "normal_debug_frag.h"
#include "normal_debug_vert.h"
#include "flat_texture_frag.h"
#include "solid_color_frag.h"
#include "default_vert.h"
#include "billboard_geo.h"
#include "billboard_vert.h"
#include "skybox_frag.h"
#include "skybox_vert.h"

#define ASPECT_RATIO (16.0f / 9.0f)
#define WINDOW_HEIGHT_IN_SCREEN_COORD 900

#define SCENE_COUNT 4

static Window window;
static GPU gpu;
static Viewport vwprt;

static Gamepad gamepad;

static uint8_t paused = 0;
void pause() { paused = 1; }
void unpause() { paused = 0; }

void handle_resize(
  uint16_t framebuffer_width,
  uint16_t framebuffer_height
);
void print_gamepad_connected(int slot);
void print_gamepad_disconnected(int slot);

void copy_shared_assets_to_gpu();

static uint8_t current_scene, previous_scene;
void switch_scene(uint8_t new_scene) {
  previous_scene = current_scene;
  current_scene = new_scene;
}

int main() {

  if (!window__create_fullscreen_game(
    "Bull",
    REQUEST_VSYNC_ON,
    // MSAA_SAMPLES_16,
    0,
    &window
  )) return 1;

  gpu__create_api(&gpu);

  gpu.enable_MSAA();
  gpu.enable_blending();
  gpu.set_default_blending();

  copy_shared_assets_to_gpu();

  window.on_focus_and_unfocus(unpause, pause);
  window.on_minimize_and_restore(pause, unpause);
  window.on_framebuffer_resize(handle_resize);
  window.on_gamepad_connect_and_disconnect(
    print_gamepad_connected,
    print_gamepad_disconnected
  );

  // TODO: shouldn't have to do this for a windowed game??? BUG?
  handle_resize(
    window.get_framebuffer_size().x,
    window.get_framebuffer_size().y
  );
  
  viewport__set_width(gpu.get_viewport_width(), &vwprt);
  viewport__set_height(gpu.get_viewport_height(), &vwprt);

  Scene loading, main_menu, connect_gamepad, action;
  loading.init = loading__init;
  loading.tick = loading__tick;
  main_menu.init = main_menu__init;
  main_menu.tick = main_menu__tick;
  connect_gamepad.init = connect_gamepad__init;
  connect_gamepad.tick = connect_gamepad__tick;
  action.init = action__init;
  action.tick = action__tick;

  Scene const *const scenes[SCENE_COUNT] = {
    &loading,
    &main_menu,
    &connect_gamepad,
    &action
  };

  for (int i = 0; i < SCENE_COUNT; i++) scenes[i]->init(&window, &vwprt, &gpu);

  current_scene = SCENE__LOADING;
  previous_scene = SCENE__LOADING;

  gpu.enable_depth_test();

  while (!window.received_closed_event()) {

    if (paused) {
      window.wait_on_events();
      continue;
    }
    
    window.poll_events();

    static double tick_start_time;
    static GameTime time;

    tick_start_time = time.sec_since_game_launch;
    time.sec_since_game_launch = window.get_seconds_since_creation();
    time.delta = time.sec_since_game_launch - tick_start_time;
    if (time.delta > DELTA_CAP) time.delta = DELTA_CAP;

    gpu.clear(&COLOR_BLACK);

    window.get_gamepad_input(&gamepad);

    if (button_was_released(BUTTON_Y, &gamepad)) break;

    if (button_was_released(BUTTON_SELECT, &gamepad)) {
      if (window.is_fullscreen()) {
        window.switch_to_windowed(
          100,
          50,
          650,
          650
        );
      } else {
        window.switch_to_fullscreen();
      }
    }

    scenes[current_scene]->tick(
      time,
      &window,
      &vwprt,
      &gpu,
      previous_scene,
      switch_scene
    );

    window.request_buffer_swap();
  }
  
  window.end();
  return 0;
}

void copy_shared_assets_to_gpu() {

  debugging__copy_gizmo_assets_to_gpu(&gpu);

  gpu.copy_points_to_gpu(&POINT);

  SKYBOX_SHADER.frag_src = SKYBOX_FRAG_SRC;
  SKYBOX_SHADER.vert_src = SKYBOX_VERT_SRC;
  gpu.copy_shader_to_gpu(&SKYBOX_SHADER);

  FLAT_TEXTURE_SHADER.frag_src = FLAT_TEXTURE_FRAG_SRC;
  FLAT_TEXTURE_SHADER.vert_src = DEFAULT_VERT_SRC;
  gpu.copy_shader_to_gpu(&FLAT_TEXTURE_SHADER);

  DEFAULT_BILLBOARD_SHADER.frag_src = FLAT_TEXTURE_FRAG_SRC;
  DEFAULT_BILLBOARD_SHADER.vert_src = BILLBOARD_VERT_SRC;
  DEFAULT_BILLBOARD_SHADER.geo_src = BILLBOARD_GEO_SRC;
  gpu.copy_shader_to_gpu(&DEFAULT_BILLBOARD_SHADER);

  SOLID_COLOR_SHADER.frag_src = SOLID_COLOR_FRAG_SRC;
  SOLID_COLOR_SHADER.vert_src = DEFAULT_VERT_SRC;
  gpu.copy_shader_to_gpu(&SOLID_COLOR_SHADER);

  NORMALS_COLOR_SHADER.frag_src = NORMAL_DEBUG_FRAG_SRC;
  NORMALS_COLOR_SHADER.vert_src = DEFAULT_VERT_SRC;
  gpu.copy_shader_to_gpu(&NORMALS_COLOR_SHADER);

  NORMALS_VIS_SHADER.frag_src = SOLID_COLOR_FRAG_SRC;
  NORMALS_VIS_SHADER.geo_src = NORMAL_DEBUG_GEO_SRC;
  NORMALS_VIS_SHADER.vert_src = NORMAL_DEBUG_VERT_SRC;
  gpu.copy_shader_to_gpu(&NORMALS_VIS_SHADER);
}

void handle_resize(uint16_t framebuffer_width, uint16_t framebuffer_height) {
  // TODO: I think there's a converstion out there for going from
  // glfw screen coords & dpi to resolution. need that here, since
  // we're using both pixels and screen coords in this math

  int viewport_origin_x_pos = 0;
  int viewport_origin_y_pos = 0;

  uint16_t viewport_width = framebuffer_width;
  uint16_t viewport_height = framebuffer_height;

  Vec2 window_dimensions = window.get_window_dim_in_screen_units();
  uint8_t window_aspect_is_wider_than_ours =
    window_dimensions.x / window_dimensions.y > ASPECT_RATIO ? 1 : 0;

  if (window_aspect_is_wider_than_ours) {
    viewport_width = viewport_height * ASPECT_RATIO;
    viewport_origin_x_pos = (window_dimensions.x - viewport_width) / 2.0f;
  } else {
    viewport_height = viewport_width / ASPECT_RATIO;
    viewport_origin_y_pos = (window_dimensions.y - viewport_height) / 2.0f;
  }

  gpu.set_viewport(
    viewport_origin_x_pos,
    viewport_origin_y_pos,
    viewport_width,
    viewport_height
  );
  viewport__set_width(viewport_width, &vwprt);
  viewport__set_height(viewport_height, &vwprt);
}

void print_gamepad_connected(int slot) {
  printf("gamepad connected! slot: %i\n", slot);
}

void print_gamepad_disconnected(int slot) {
  printf("gamepad disconnected! slot: %i\n", slot);
}