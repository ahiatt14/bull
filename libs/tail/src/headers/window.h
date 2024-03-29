#ifndef __TAIL_WINDOW__
#define __TAIL_WINDOW__

#include <stdint.h>

#include "vector.h"
#include "gamepad.h"

#define REQUEST_VSYNC_ON 1
#define REQUEST_VSYNC_OFF 0

#define MSAA_SAMPLES_0 0
#define MSAA_SAMPLES_2 2
#define MSAA_SAMPLES_4 4
#define MSAA_SAMPLES_8 8
#define MSAA_SAMPLES_16 16

#define REQUEST_FULLSCREEN 1
#define REQUEST_WINDOWED 0

typedef struct TAILWINDOW {
  
  void (*on_minimize_and_restore)(
    void (*handle_minimize)(),
    void (*handle_restore)()
  );
  void (*on_focus_and_unfocus)(
    void (*handle_focus)(),
    void (*handle_unfocus)()
  );
  void (*on_framebuffer_resize)(
    void (*handle_resize)(
      uint16_t width_in_pixels,
      uint16_t height_in_pixels
    )
  );
  void (*on_gamepad_connect_and_disconnect)(
    void (*handle_gamepad_connect)();
    void (*handle_gamepad_disconnect)();
  );

  uint8_t (*gamepad_is_connected)();
  void (*get_gamepad_input)(
    Gamepad *const gamepad
  );

  uint8_t (*is_fullscreen)();
  Vec2 (*get_window_dim_in_screen_units)();
  Vec2 (*get_framebuffer_size)();
  double (*get_seconds_since_creation)();
  void (*switch_to_fullscreen)();
  void (*switch_to_windowed)(
    uint16_t position_x,
    uint16_t position_y,
    uint16_t width_in_screen_units,
    uint16_t height_in_screen_units
  );
  void (*enable_vsync)();
  void (*disable_vsync)();

  void (*poll_events)();
  void (*wait_on_events)();
  void (*request_buffer_swap)();
  uint8_t (*received_closed_event)();
  void (*end)();
} Window;

uint8_t window__create_fullscreen_game(
  const char *name,
  uint8_t request_vsync,
  uint8_t MSAA_samples,
  Window *const window
);

uint8_t window__create_windowed_game(
  uint16_t win_width,
  uint16_t win_height,
  uint16_t pos_x,
  uint16_t pos_y,
  const char *name,
  uint8_t vsync,
  uint8_t request_MSAA,
  Window *const window
);

#endif