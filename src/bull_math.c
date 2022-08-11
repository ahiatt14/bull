#include <math.h>
#include <stdint.h>

#include "bull_math.h"

const struct m4x4* camera__calculate_ortho(
  struct viewport *const vwprt,
  struct camera *const cam
) {
  static float r, l, t, b;
  static float m0, m5, m10, m12, m13, m14;

  r = vwprt->_width * 0.5f;
  l = -r;
  t = r / viewport__get_aspect_ratio(vwprt);
  b = -t; 

  m0 = 2.0f / (r - l);
  m5 = 2.0f / (t - b);
  m10 = -2.0f / (cam->far_clip_distance - cam->near_clip_distance);
  m12 = -(r + l) / (r - l);
  m13 = -(t + b) / (t - b);
  m14 = 
    -(cam->far_clip_distance + cam->near_clip_distance) /
    (cam->far_clip_distance - cam->near_clip_distance);

  m4x4__create(
    m0, 0, 0, m12,
    0, m5, 0, m13,
    0, 0, m10, m14,
    0, 0, 0, 1,
    &cam->projection
  );

  return &cam->projection;
}

char array_contains_u_i(
  unsigned int value,
  unsigned int *values,
  int count
) {
  for (int i = 0; i < count; i++) if (values[i] == value) return 1;
  return 0;
}

struct vec2 vec2__turn_90_deg(
  uint8_t left,
  struct vec2 src
) {
  return left ?
    ((struct vec2){ -src.y, src.x }) : 
    ((struct vec2){ src.y, -src.x });
}

// TODO: make this work for arbitrary origins
uint8_t is_moving_cw_around_world_up(
  struct vec3 position,
  struct vec3 previous_position
) {
  struct vec3 previous_to_current_position = vec3_minus_vec3(
    position,
    previous_position
  );
  struct vec3 cross = vec3__cross(
    position,
    previous_to_current_position
  );
  return cross.y <= 0 ? 1 : 0;
}

// TODO: bug here. we get correct cw/ccw facing
// when the player is moving, but when the player stops it always faces
// back one direction
float find_cw_or_ccw_facing_around_world_up(
  struct vec3 position,
  struct vec3 previous_position
) {
  float ccw_angle_in_deg = rad_to_deg(atan(
    -position.z /
    position.x
  ));
  if (position.x < 0) ccw_angle_in_deg += 180;
  return
    is_moving_cw_around_world_up(
      position,
      previous_position
    ) ?
    ccw_angle_in_deg + 180 :
    ccw_angle_in_deg;
}

float rads_from_arc_len_and_radius(
  float arc_length,
  float radius
) {
  return arc_length / radius;
}

float loop_float(float v, float min, float max) {
  if (v < min) return max - min - v;
  if (v > max) return min + v - max;
  return v;
}

