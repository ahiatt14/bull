#ifndef __BULL_MENU__
#define __BULL_MENU__

#include "tail.h"
#include "scene.h"

void menu__init(window_api *window, const gpu_api *gpu);
void menu__tick(
  double seconds_since_creation,
  const viewport *vw,
  const gpu_api *gpu,
  struct scene **scenes,
  void switch_scene(struct scene* new_scene)
);

#endif