#ifndef __BULL_SCENE__
#define __BULL_SCENE__

struct scene;

struct scene {
  void (*init)(
    window_api *window,
    const gpu_api *gpu
  );
  void (*tick)(
    double seconds_since_creation,
    const viewport *vwprt,
    const gpu_api *gpu,
    struct scene **scenes,
    void (*switch_scene)(struct scene* new_scene)
  );
};

#endif