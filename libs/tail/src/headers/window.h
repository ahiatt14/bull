#ifndef __TAIL_WINDOW__
#define __TAIL_WINDOW__

#include <stdint.h>
#include "input.h"

struct window_api {
  double (*get_seconds_since_creation)();
  // TODO: implement!
  void (*register_listener_for_gamepad_connect)(
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
    void (*handle_resize)(uint16_t width, uint16_t height)
  );
  void (*request_buffer_swap)();
  void (*get_gamepad_input)(struct gamepad_input *const input);
  uint8_t (*gamepad_is_connected)();
};

uint8_t window__create(
  uint16_t window_width,
  uint16_t window_height,
  const char *name,
  uint8_t vsync,
  // unsigned short int fullscreen, TODO: add support
  struct window_api *const window
);

// TODO: these could be attached to the struct like the rest,
// dunno why they aren't?
uint8_t window__received_closed_event();
void window__poll_events();
void window__end();

#endif