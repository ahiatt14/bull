#ifndef __TAIL_WINDOW__
#define __TAIL_WINDOW__

#include "input.h"

struct window_api {
  double (*get_seconds_since_creation)();
  // TODO: implement!
  void (*register_listener_for_gamepad_connect_event)(
    void (*handle_gamepad_connect)();
    void (*handle_gamepad_disconnect)();
  );
  void (*register_listener_for_minimize)(
    void (*handle_minimize)(),
    void (*handle_restore)()
  );
  void (*register_listener_for_focus)(
    void (*handle_focus)(),
    void (*handle_unfocus)()
  );
  void (*register_listener_for_resize)(
    void (*handle_resize)(int width, int height)
  );
  void (*request_buffer_swap)();
  void (*get_gamepad_input)(struct gamepad_input *const input);
};

unsigned short int window__create(
  int window_width,
  int window_height,
  const char *name,
  unsigned short int vsync,
  // unsigned short int fullscreen, TODO: add support
  struct window_api *const window
);

unsigned short int window__received_closed_event();
void window__poll_events();
void window__end();

#endif