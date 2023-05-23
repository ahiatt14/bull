#ifndef __BULL_SCENE__
#define __BULL_SCENE__

#include "tail.h"

#include "constants.h"
#include "tail_helpers.h"

#define SCENE__LOADING 0
#define SCENE__MAIN_MENU 1
#define SCENE__CONNECT_GAMEPAD 2
#define SCENE__ACTION 3

typedef struct BULLSCENE Scene;

struct BULLSCENE {
  void (*set_previous_scene)(Scene const *previous_scene);
  void (*init)(
    Window const *const window,
    Viewport *const vwprt,
    GPU const *const gpu
  );
  void (*tick)(
    GameTime time,
    Window const *const window,
    Viewport *const vwprt,
    GPU const *const gpu,
    uint8_t previous_scene,
    void (*switch_scene)(uint8_t new_scene)
  );
};

// TODO: take another crack at removing this redundancy!
void main_menu__init(
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu
);
void main_menu__tick(
  GameTime time,
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu,
  uint8_t previous_scene,
  void (*switch_scene)(uint8_t new_scene)
);

void action__init(
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu
);
void action__tick(
  GameTime time,
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu,
  uint8_t previous_scene,
  void (*switch_scene)(uint8_t new_scene)
);

void pause__init(
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu
);
void pause__tick(
  GameTime time,
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu,
  uint8_t previous_scene,
  void (*switch_scene)(uint8_t new_scene)
);

void loading__init(
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu
);
void loading__tick(
  GameTime time,
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu,
  uint8_t previous_scene,
  void (*switch_scene)(uint8_t new_scene)
);

void connect_gamepad__init(
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu
);
void connect_gamepad__tick(
  GameTime time,
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu,
  uint8_t previous_scene,
  void (*switch_scene)(uint8_t new_scene)
);

void ocean__init(
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu
);
void ocean__tick(
  GameTime time,
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu,
  uint8_t previous_scene,
  void (*switch_scene)(uint8_t new_scene)
);

#endif