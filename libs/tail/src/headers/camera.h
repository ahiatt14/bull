#ifndef __TAIL_CAMERA__
#define __TAIL_CAMERA__

#include "viewport.h"
#include "m4x4.h"
#include "vector.h"

typedef struct TAILCAMERA {
  M4x4 lookat;
  M4x4 projection;
  Vec3 position;
  Vec3 look_target;
  float near_clip_distance;
  float far_clip_distance;
  float horizontal_fov_in_deg;
} Camera;

void camera__init(
  Camera *c
);
const M4x4* camera__calculate_lookat(
  Vec3 world_up,
  Camera *const c
);
const M4x4* camera__calculate_perspective(
  Viewport *const vwprt,
  Camera *const c
);

#endif