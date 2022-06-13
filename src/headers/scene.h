#ifndef __BULL_SCENE__
#define __BULL_SCENE__

struct scene;

struct scene {
  void (*init)(
    struct window_api *window,
    struct viewport *vwprt,
    struct gpu_api *gpu
  );
  void (*tick)(
    double seconds_since_creation,
    const struct viewport *vwprt,
    struct gpu_api *gpu,
    struct scene **scenes,
    void (*switch_scene)(struct scene* new_scene)
  );
};

#endif