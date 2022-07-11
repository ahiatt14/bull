#include <stdio.h>
#include <stdint.h>

#include "tail.h"
#include "scene.h"

#define ASPECT_RATIO 1
#define REQUEST_VSYNC_ON 1
#define REQUEST_VSYNC_OFF 0

#define SCENE_COUNT 3

static struct window_api window;
static struct gpu_api gpu;
static struct viewport vwprt;

static uint8_t paused = 0;
void pause() { paused = 1; }
void unpause() { paused = 0; }

void handle_resize(uint16_t framebuffer_width, uint16_t framebuffer_height) {

  // TODO: letterboxing!
  // if 

  gpu.set_viewport(0, 0, framebuffer_width, framebuffer_height);
  viewport__set_width(framebuffer_width, &vwprt);
  viewport__set_height(framebuffer_height, &vwprt);
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
    800,
    800,
    "BULL",
    REQUEST_VSYNC_ON,
    &window
  )) return 1;
  gpu__create_api(&gpu);

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
  struct scene arena_scene;
  struct scene connect_gamepad;
  main_menu_scene.init = main_menu__init;
  main_menu_scene.tick = main_menu__tick;
  arena_scene.init = arena__init;
  arena_scene.tick = arena__tick;
  connect_gamepad.init = connect_gamepad__init;
  connect_gamepad.tick = connect_gamepad__tick;

  struct scene const *const scenes[SCENE_COUNT] = {
    &main_menu_scene,
    &arena_scene,
    &connect_gamepad
  };

  for (int i = 0; i < SCENE_COUNT; i++)
    scenes[i]->init(&window, &vwprt, &gpu);

  current_scene = SCENE__MAIN_MENU;
  previous_scene = SCENE__MAIN_MENU;

  gpu.enable_depth_test();

  while (!window__received_closed_event()) {
    // TODO: maybe we can sleep on pause instead of just zooming this loop
    // while doing no work? look into that
    if (!paused) {      
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
    window__poll_events();
  }
  
  window__end();
  return 0;
}