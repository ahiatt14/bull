#ifndef __TAIL_DEBUGGING__
#define __TAIL_DEBUGGING__

#include "gpu.h"
#include "vector.h"
#include "space.h"
#include "camera.h"

void debugging__copy_gizmo_assets_to_gpu(
  struct GPU const *const gpu
);

void debugging__draw_space_gizmo(
  struct GPU const *const gpu,
  struct Camera const *const cam,
  struct CoordinateSpace const *const space,
  struct Vec3 position
);

#endif