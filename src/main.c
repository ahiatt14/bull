#include <stdio.h>
#include <stdint.h>

#include "tail.h"
#include "scene.h"

#define ASPECT_RATIO 1.0f/1.0f
#define REQUEST_VSYNC_ON 1

#define SCENE_COUNT 3

static struct window_api window;
static struct gpu_api gpu;
static struct viewport vwprt;

static uint8_t paused = 0;
void pause() { paused = 1; }
void unpause() { paused = 0; }

void handle_resize(int w, int h) {
  gpu.set_viewport(0, 0, w, h);
  viewport__set_width(w, &vwprt);
  viewport__set_height(h, &vwprt);
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

  window.register_listener_for_focus(unpause, pause);
  window.register_listener_for_minimize(pause, unpause);
  window.register_listener_for_resize(handle_resize);
  window.register_listener_for_gamepad_connect(
    print_gamepad_connected,
    print_gamepad_disconnected
  );

  gpu__create_api(&gpu);
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
    if (!paused) {      
      scenes[current_scene]->tick(
        &window,
        &vwprt,
        &gpu,
        previous_scene,
        switch_scene
      );
      window.request_buffer_swap();            
    }
    window__poll_events();
  }
  
  window__end();
  return 0;
}