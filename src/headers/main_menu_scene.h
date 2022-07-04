#ifndef __BULL_MENU__
#define __BULL_MENU__

#include "tail.h"
#include "scene.h"

void main_menu__init(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu
);
void main_menu__tick(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu,
  struct scene const *const *const scenes,
  void (*switch_scene)(struct scene const *const new_scen)
);

#endif