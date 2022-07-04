#ifndef __BULL_SCENE__
#define __BULL_SCENE__

#include "tail.h"

struct scene;

struct scene {
  void (*init)(
    struct window_api const *const window,
    struct viewport *const vwprt,
    struct gpu_api const *const gpu
  );
  void (*tick)(
    struct window_api const *const window,
    struct viewport *const vwprt,
    struct gpu_api const *const gpu,
    struct scene const *const *const scenes,
    void (*switch_scene)(struct scene const *const new_scen)
  );
};

#endif