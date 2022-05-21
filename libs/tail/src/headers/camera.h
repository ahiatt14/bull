#ifndef __TAIL_CAMERA__
#define __TAIL_CAMERA__

#include "viewport.h"
#include "m4x4.h"
#include "vec3.h"

struct camera {
  struct m4x4 _lookat;
  struct m4x4 _perspective;
  struct vec3 _position;
  struct vec3 _look_target;
  float _near_clip_distance;
  float _far_clip_distance;
  float _horizontal_fov_in_deg;
  short int _lookat_needs_recalculating;
  short int _perspective_needs_recalculating;
};

void camera__init(struct camera *c);
const struct m4x4* camera__calculate_lookat(
  const struct vec3 *world_up,
  struct camera *c
);
const struct m4x4* camera__calculate_perspective(
  struct viewport *vwprt,
  struct camera *c
);
const struct m4x4* camera__get_lookat(const struct camera *c);
const struct m4x4* camera__get_perspective(const struct camera *c);
const struct vec3* camera__get_position(struct camera *c);
const struct vec3* camera__get_look_target(struct camera *c);
float camera__get_horizontal_fov_in_deg(const struct camera *c);
void camera__set_position(float x, float y, float z, struct camera *c);
void camera__set_look_target(const struct vec3 *t, struct camera *c);
void camera__set_horizontal_fov_in_deg(float fov, struct camera *c);
void camera__set_near_clip_distance(float n, struct camera *c);
void camera__set_far_clip_distance(float f, struct camera *c);
short int camera__lookat_needs_recalculating(const struct camera *c);
short int camera__perspective_needs_recalculating(const struct camera *c);

#endif