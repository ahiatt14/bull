#ifndef __BULL_SCENE__
#define __BULL_SCENE__

#include "tail.h"

#define SCENE__MAIN_MENU 0
#define SCENE__ACTION 1
#define SCENE__CONNECT_GAMEPAD 2

struct scene;

struct scene {
  void (*set_previous_scene)(struct scene const *previous_scene);
  void (*init)(
    struct window_api const *const window,
    struct viewport *const vwprt,
    struct gpu_api const *const gpu
  );
  void (*tick)(
    struct window_api const *const window,
    struct viewport *const vwprt,
    struct gpu_api const *const gpu,
    uint8_t previous_scene,
    void (*switch_scene)(uint8_t new_scene)
  );
};

// TODO: take another crack at removing this redundancy!
void main_menu__init(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu
);
void main_menu__tick(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu,
  uint8_t previous_scene,
  void (*switch_scene)(uint8_t new_scene)
);

void action__init(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu
);
void action__tick(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu,
  uint8_t previous_scene,
  void (*switch_scene)(uint8_t new_scene)
);

void connect_gamepad__init(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu
);
void connect_gamepad__tick(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu,
  uint8_t previous_scene,
  void (*switch_scene)(uint8_t new_scene)
);

void ocean__init(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu
);
void ocean__tick(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu,
  uint8_t previous_scene,
  void (*switch_scene)(uint8_t new_scene)
);

#endif