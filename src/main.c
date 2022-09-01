#include <stdio.h>
#include <stdint.h>

#include "tail.h"
#include "constants.h"
#include "scene.h"

#include "normal_debug_geo.h"
#include "normal_debug_frag.h"
#include "normal_debug_vert.h"
#include "flat_texture_frag.h"
#include "alpha_texture_frag.h"
#include "solid_color_frag.h"
#include "default_vert.h"

#define ASPECT_RATIO (4.0f / 3.0f)
#define WINDOW_HEIGHT_IN_SCREEN_COORD 900

#define SCENE_COUNT 4

static struct window_api window;
static struct gpu_api gpu;
static struct viewport vwprt;

static struct gamepad_input gamepad;

static uint8_t paused = 0;
void pause() { paused = 1; }
void unpause() { paused = 0; }

void copy_shared_assets_to_gpu();

void handle_resize(uint16_t framebuffer_width, uint16_t framebuffer_height) {
  // TODO: I think there's a converstion out there for going from
  // glfw screen coords & dpi to resolution. need that here, since
  // we're using both pixels and screen coords in this math

  int viewport_origin_x_pos = 0;
  int viewport_origin_y_pos = 0;

  uint16_t viewport_width = framebuffer_width;
  uint16_t viewport_height = framebuffer_height;

  struct vec2 window_dimensions = window.get_window_dimensions();
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

static uint8_t current_scene, previous_scene;
void switch_scene(uint8_t new_scene) {
  previous_scene = current_scene;
  current_scene = new_scene;
}

int main() {

  if (!window__create(
    WINDOW_HEIGHT_IN_SCREEN_COORD * ASPECT_RATIO,
    WINDOW_HEIGHT_IN_SCREEN_COORD,
    50,
    50,
    "Greathawk",
    REQUEST_VSYNC_ON,
    REQUEST_WINDOWED,
    REQUEST_MSAA_ON,
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
  
  viewport__set_width(gpu.get_viewport_width(), &vwprt);
  viewport__set_height(gpu.get_viewport_height(), &vwprt);

  struct scene main_menu_scene;
  struct scene action_scene;
  struct scene connect_gamepad;
  struct scene ocean; 
  main_menu_scene.init = main_menu__init;
  main_menu_scene.tick = main_menu__tick;
  action_scene.init = action__init;
  action_scene.tick = action__tick;
  connect_gamepad.init = connect_gamepad__init;
  connect_gamepad.tick = connect_gamepad__tick;
  ocean.init = ocean__init;
  ocean.tick = ocean__tick;

  struct scene const *const scenes[SCENE_COUNT] = {
    &main_menu_scene,
    &action_scene,
    &connect_gamepad,
    &ocean
  };

  // for (int i = 0; i < SCENE_COUNT; i++)
  //   scenes[i]->init(&window, &vwprt, &gpu);

  // TODO: temp for testing
  scenes[SCENE__OCEAN]->init(&window, &vwprt, &gpu);

  // current_scene = SCENE__MAIN_MENU;
  // previous_scene = SCENE__MAIN_MENU;

  // TODO: temp for testing
  current_scene = SCENE__OCEAN;
  previous_scene = SCENE__OCEAN;

  gpu.enable_depth_test();

  while (!window.received_closed_event()) {
    window.poll_events();
    // TODO: maybe we can sleep on pause instead of just zooming this loop
    // while doing no work? seems like we can use
    // glfwWaitEvents & glfwPostEmptyEvent
    if (!paused) {

      gpu.clear(&COLOR_BLACK);

      window.get_gamepad_input(&gamepad);

      // TODO: ultimately don't like this fn's API.
      // should be passing in the gamepad struct
      if (button_was_released(
        BUTTON_SELECT,
        gamepad.buttons,
        gamepad.previous_buttons
      )) {
        if (window.is_fullscreen()) {
          window.switch_to_windowed();
        } else {
          window.switch_to_fullscreen();
        }
      }

      scenes[current_scene]->tick(
        &window,
        &vwprt,
        &gpu,
        previous_scene,
        switch_scene
      );         
    }
    // TODO: we put this out here because if there's no
    // buffer swapping while the game is paused, opengl
    // doesn't sync with the cpu and this window loop runs loose,
    // eating up cpu
    window.request_buffer_swap();
  }
  
  window.end();
  return 0;
}

void copy_shared_assets_to_gpu() {

  FLAT_TEXTURE_SHADER.frag_shader_src = flat_texture_frag_src;
  FLAT_TEXTURE_SHADER.vert_shader_src = default_vert_src;
  gpu.copy_shader_to_gpu(&FLAT_TEXTURE_SHADER);

  ALPHA_TEXTURE_SHADER.frag_shader_src = alpha_texture_frag_src;
  ALPHA_TEXTURE_SHADER.vert_shader_src = default_vert_src;
  gpu.copy_shader_to_gpu(&ALPHA_TEXTURE_SHADER);

  SOLID_COLOR_SHADER.frag_shader_src = solid_color_frag_src;
  SOLID_COLOR_SHADER.vert_shader_src = default_vert_src;
  gpu.copy_shader_to_gpu(&SOLID_COLOR_SHADER);

  NORMALS_COLOR_SHADER.frag_shader_src = normal_debug_frag_src;
  NORMALS_COLOR_SHADER.vert_shader_src = default_vert_src;
  gpu.copy_shader_to_gpu(&NORMALS_COLOR_SHADER);

  NORMALS_VIS_SHADER.frag_shader_src = solid_color_frag_src;
  NORMALS_VIS_SHADER.geo_shader_src = normal_debug_geo_src;
  NORMALS_VIS_SHADER.vert_shader_src = normal_debug_vert_src;
  gpu.copy_shader_to_gpu(&NORMALS_VIS_SHADER);

  gpu.copy_static_mesh_to_gpu(&QUAD);
}