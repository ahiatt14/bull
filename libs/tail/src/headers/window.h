#ifndef __TAIL_WINDOW__
#define __TAIL_WINDOW__

typedef struct WINDOW_API {
  double (*get_seconds_since_creation)();
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
} window_api;

window_api* window__create(
  int window_width,
  int window_height,
  const char *name,
  unsigned short int vsync
  // unsigned short int fullscreen, TODO: add support
);

unsigned short int window__received_closed_event();
void window__poll_events();
void window__end();

#endif