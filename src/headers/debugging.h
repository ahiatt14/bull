#ifndef __BULL_DEBUGGING__
#define __BULL_DEBUGGING__

#include "tail.h"

struct coord_gizmo {
  struct vec3 position;
  struct coordinate_space space;
};

void coord_gizmo__copy_assets_to_gpu(
  struct gpu_api const *const gpu
);

void coord_gizmo__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct coord_gizmo const *const gizmo
);

#endif