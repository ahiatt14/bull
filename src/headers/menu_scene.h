#ifndef __BULL_MENU__
#define __BULL_MENU__

#include "tail.h"
#include "scene.h"

void menu__init(
  struct window_api *window,
  struct viewport *vwprt,
  struct gpu_api *gpu
);
void menu__tick(
  double seconds_since_creation,
  const struct viewport *vw,
  struct gpu_api *gpu,
  struct scene **scenes,
  void switch_scene(struct scene* new_scene)
);

#endif