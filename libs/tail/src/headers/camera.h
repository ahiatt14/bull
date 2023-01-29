#ifndef __TAIL_CAMERA__
#define __TAIL_CAMERA__

#include "viewport.h"
#include "m4x4.h"
#include "vector.h"

struct Camera {
  struct M4x4 lookat;
  struct M4x4 projection;
  struct Vec3 position;
  struct Vec3 look_target;
  float near_clip_distance;
  float far_clip_distance;
  float horizontal_fov_in_deg;
};

void camera__init(struct Camera *c);
const struct M4x4* camera__calculate_lookat(
  struct Vec3 world_up,
  struct Camera *const c
);
const struct M4x4* camera__calculate_perspective(
  struct Viewport *const vwprt,
  struct Camera *const c
);

#endif