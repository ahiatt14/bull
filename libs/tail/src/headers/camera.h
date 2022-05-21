#ifndef __TAIL_CAMERA__
#define __TAIL_CAMERA__

#include "viewport.h"
#include "m4x4.h"
#include "vec3.h"

typedef struct CAMERA {
  m4x4 _lookat;
  m4x4 _perspective;
  vec3 _position;
  vec3 _look_target;
  float _near_clip_distance;
  float _far_clip_distance;
  float _horizontal_fov_in_deg;
  short int _lookat_needs_recalculating;
  short int _perspective_needs_recalculating;
} camera;

void camera__init(camera *c);
const m4x4* camera__calculate_lookat(const vec3 *world_up, camera *c);
const m4x4* camera__calculate_perspective(viewport *vwprt, camera *c);
const m4x4* camera__get_lookat(const camera *c);
const m4x4* camera__get_perspective(const camera *c);
const vec3* camera__get_position(camera *c);
const vec3* camera__get_look_target(camera *c);
float camera__get_horizontal_fov_in_deg(const camera *c);
void camera__set_position(float x, float y, float z, camera *c);
void camera__set_look_target(const vec3 *t, camera *c);
void camera__set_horizontal_fov_in_deg(float fov, camera *c);
void camera__set_near_clip_distance(float n, camera *c);
void camera__set_far_clip_distance(float f, camera *c);
short int camera__lookat_needs_recalculating(const camera *c);
short int camera__perspective_needs_recalculating(const camera *c);

#endif