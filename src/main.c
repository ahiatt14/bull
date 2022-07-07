#include <stdio.h>

#include "tail.h"
#include "scene.h"

#define REQUEST_VSYNC_ON 1

struct window_api window;
struct gpu_api gpu;
struct viewport vwprt;

unsigned short int paused = 0;
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

struct scene const *current_scene;
void switch_scene(struct scene const *const new_scene) {
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
  // TODO: rename for consistency
  window.register_listener_for_gamepad_connect_event(
    print_gamepad_connected,
    print_gamepad_disconnected
  );

  gpu__create_api(&gpu);
  viewport__set_width(gpu.get_viewport_width(), &vwprt);
  viewport__set_height(gpu.get_viewport_height(), &vwprt);

  struct scene main_menu_scene;
  struct scene arena_scene;
  main_menu_scene.init = main_menu__init;
  main_menu_scene.tick = main_menu__tick;
  arena_scene.init = arena__init;
  arena_scene.tick = arena__tick;

  struct scene const *const scenes[2] = {
    &main_menu_scene, &arena_scene
  };

  current_scene = &main_menu_scene;

  current_scene->init(&window, &vwprt, &gpu);

  gpu.enable_depth_test();

  while (!window__received_closed_event()) {
    if (!paused) {
      current_scene->tick(
        &window,
        &vwprt,
        &gpu,
        scenes,
        switch_scene
      );
      window.request_buffer_swap();            
    }
    window__poll_events();
  }
  
  window__end();
  return 0;
}