#ifndef __TAIL_DEBUGGING__
#define __TAIL_DEBUGGING__

#include "gpu.h"
#include "vector.h"
#include "space.h"
#include "camera.h"

void debugging__copy_gizmo_assets_to_gpu(
  struct gpu_api const *const gpu
);

void debugging__draw_space_gizmo(
  struct gpu_api const *const gpu,
  struct camera const *const cam,
  struct coordinate_space const *const space,
  struct vec3 position
);

#endif