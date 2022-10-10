#include <math.h>
#include <stdint.h>

#include "bull_math.h"
#include "constants.h"

const struct m4x4* camera__calculate_ortho(
  float r,
  float t,
  float n,
  float f,
  struct camera *const cam
) {
  m4x4__create(
    1 / r, 0, 0, 0,
    0, 1 / t, 0, 0,
    0, 0, -2.0f / (f - n), -(f + n) / (f - n),
    0, 0, 0, 1,
    &cam->projection
  );

  return &cam->projection;
}

struct vec2 vec2__turn_90_deg(
  uint8_t left,
  struct vec2 src
) {
  // TODO: we can use the left param to arithmetically
  // flip these vec2 components, avoid conditional
  return left ?
    ((struct vec2){ -src.y, src.x }) : 
    ((struct vec2){ src.y, -src.x });
}

uint8_t is_moving_cw_around_world_up(
  struct vec3 position,
  struct vec3 previous_position
) {
  struct vec3 cross = vec3__cross(
    position,
    vec3_minus_vec3(position, previous_position)
  );
  return cross.y <= 0 ? 1 : 0;
}

float battlefield_deg_from_world_pos(
  struct vec3 t
) {
  // TODO: yikes, clean up / optimize
  float deg = rad_to_deg(atan(-t.x / -t.z));
  if (t.x < 0 && t.z >= 0) {
    deg += 180;
  } else if (t.x >= 0 && t.z >= 0) {
    deg += 180;
  } else if (t.x >= 0 && t.z < 0) {
    deg += 360;
  }
  return deg;
}

// float lerp_around_battlefield(
//   double delta_time,
//   struct revolution rev,
//   float deg,
// ) {
  
// }

float find_cw_or_ccw_facing_around_world_up(
  struct vec3 position,
  struct vec3 previous_position
) {
  float ccw_angle_in_deg = battlefield_deg_from_world_pos(position);
  return
    is_moving_cw_around_world_up(
      position,
      previous_position
    ) ?
    ccw_angle_in_deg + 270 :
    ccw_angle_in_deg + 90;
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

struct vec3 slide_along_radius_around_world_origin(
  float radius,
  struct vec3 projected_pos,
  struct vec3 pos
) {
  struct vec3 pos_at_radius = scalar_x_vec3(
    radius / vec3__distance(pos, ORIGIN),
    pos
  );
  float distance_left = vec3__distance(projected_pos, pos_at_radius);
  float rads = rads_from_arc_len_and_radius(distance_left, radius);
  if (is_moving_cw_around_world_up(projected_pos, pos)) rads = -rads;
  struct m4x4 rotation = {0};
  m4x4__rotation(rads, WORLDSPACE.up, &rotation);
  return m4x4_x_point(&rotation, pos_at_radius);
}

struct vec3 battlefield_to_world_pos(
  struct battlefield_pos position
) {
  struct m4x4 rotation = M4X4_IDENTITY;
  m4x4__rotation(deg_to_rad(position.degrees), WORLDSPACE.up, &rotation);
  return m4x4_x_point(
    &rotation,
    (struct vec3){ 0, 0, -position.radius }
  );
}

struct battlefield_pos world_to_battlefield_pos(
  struct vec3 position
) {
  return (struct battlefield_pos){
    .radius = vec3__distance(position, ORIGIN),
    .degrees = battlefield_deg_from_world_pos(position)
  };
}