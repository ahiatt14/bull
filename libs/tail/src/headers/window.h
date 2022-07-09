#ifndef __TAIL_WINDOW__
#define __TAIL_WINDOW__

#include <stdint.h>

#include "vector.h"
#include "input.h"

struct window_api {
  
  void (*on_minimize_and_restore)(
    void (*handle_minimize)(),
    void (*handle_restore)()
  );
  void (*on_focus_and_unfocus)(
    void (*handle_focus)(),
    void (*handle_unfocus)()
  );
  void (*on_framebuffer_resize)(
    void (*handle_resize)(uint16_t width, uint16_t height)
  );
  void (*on_gamepad_connect_and_disconnect)(
    void (*handle_gamepad_connect)();
    void (*handle_gamepad_disconnect)();
  );

  struct vec2 (*get_window_dimensions)();
  double (*get_seconds_since_creation)();
  void (*request_buffer_swap)();
  uint8_t (*gamepad_is_connected)();
  void (*get_gamepad_input)(struct gamepad_input *const input);
};

uint8_t window__create(
  uint16_t window_width,
  uint16_t window_height,
  const char *name,
  uint8_t vsync,
  // uint8_t fullscreen, TODO: add support
  struct window_api *const window
);

// TODO: these could be attached to the struct like the rest,
// dunno why they aren't?
uint8_t window__received_closed_event();
void window__poll_events();
void window__end();

#endif