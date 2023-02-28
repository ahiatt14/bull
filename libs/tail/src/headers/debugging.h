#ifndef __TAIL_DEBUGGING__
#define __TAIL_DEBUGGING__

#include "gpu.h"
#include "vector.h"
#include "space.h"
#include "camera.h"

void debugging__copy_gizmo_assets_to_gpu(
  GPU const *const gpu
);

void debugging__draw_space_gizmo(
  GPU const *const gpu,
  Camera const *const cam,
  CoordinateSpace const *const space,
  Vec3 position
);

#endif