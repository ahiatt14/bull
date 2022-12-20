#ifndef __TAIL_WINDOW__
#define __TAIL_WINDOW__

#include <stdint.h>

#include "vector.h"
#include "gamepad.h"

#define REQUEST_VSYNC_ON 1
#define REQUEST_VSYNC_OFF 0

#define REQUEST_MSAA_ON 1
#define REQUEST_MSAA_OFF 0

#define REQUEST_FULLSCREEN 1
#define REQUEST_WINDOWED 0

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

  uint8_t (*gamepad_is_connected)();
  void (*get_gamepad_input)(struct gamepad_input *const gamepad);

  uint8_t (*is_fullscreen)();
  struct vec2 (*get_window_dimensions)();
  double (*get_seconds_since_creation)();  
  void (*switch_to_fullscreen)();
  void (*switch_to_windowed)();

  void (*poll_events)();
  void (*request_buffer_swap)();
  uint8_t (*received_closed_event)();
  void (*end)();
};

uint8_t window__create(
  uint16_t win_width,
  uint16_t win_height,
  uint16_t pos_x,
  uint16_t pos_y,
  const char *title,
  uint8_t vsync,
  uint8_t fullscreen,
  uint8_t request_MSAA,
  struct window_api *const window
);

#endif