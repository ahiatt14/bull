#ifndef __TAIL_CAMERA__
#define __TAIL_CAMERA__

#include "viewport.h"
#include "m4x4.h"
#include "vector.h"

struct camera {
  struct m4x4 lookat;
  struct m4x4 perspective;
  struct vec3 position;
  struct vec3 look_target;
  float near_clip_distance;
  float far_clip_distance;
  float horizontal_fov_in_deg;
};

void camera__init(struct camera *c);
const struct m4x4* camera__calculate_lookat(
  struct vec3 world_up,
  struct camera *const c
);
const struct m4x4* camera__calculate_perspective(
  struct viewport *const vwprt,
  struct camera *const c
);

#endif