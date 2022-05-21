#include "tail.h"
#include "scene.h"
#include "menu_scene.h"

#define REQUEST_VSYNC_ON 1

unsigned short int paused = 0;
void pause() { paused = 1; }
void unpause() { paused = 0; }

window_api *window;
const gpu_api *gpu;

viewport vwprt;
void handle_resize(int w, int h) {
  gpu->set_viewport(0, 0, w, h);
  viewport__set_width(w, &vwprt); // see note A
  viewport__set_height(h, &vwprt);
}

struct scene *scenes[2];
struct scene *current_scene;
struct scene menu_scene;
struct scene play_scene;
void switch_scene(struct scene* new_scene) { current_scene = new_scene; }

int main() {

  window = window__create(
    800,
    800,
    "BULL",
    REQUEST_VSYNC_ON
  );

  window->register_listener_for_focus(unpause, pause);
  window->register_listener_for_minimize(pause, unpause);
  window->register_listener_for_resize(handle_resize);

  gpu = gpu__create_api();

  menu_scene.init = menu__init;
  menu_scene.tick = menu__tick;

  while (!window__received_closed_event()) {
    
    window->request_buffer_swap();
    window__poll_events();
  }
  
  window__end();
  return 0;
}

/*

  NOTES  NOTES  NOTES  NOTES  NOTES  NOTES  

  Note A) TODO: instead of checking if projection needs
  recalc every frame, we could assign diff fn pnters
  in these fns, and the client scene can call
  the "possibly recalc perspective" fn every frame, it just
  might be noop
  also check why we can't calculate persp when these are set?
  (I think persp calc is in camera maybe..)
*/