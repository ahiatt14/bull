#include <math.h>

#include "tail.h"

#include "billboard.h"

#include "constants.h"
#include "bull_math.h"

void billboard__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct billboard *const bb
) {

  static struct vec3 new_forward;
  new_forward = vec3_minus_vec3(cam->position, bb->transform.position);
  
  static float up_rotation_theta, right_rotation_theta;
  static struct quaternion up_rotation, right_rotation;

  up_rotation_theta = atan(new_forward.z / new_forward.x);
  if (new_forward.x > 0) up_rotation_theta += M_PI;
  up_rotation= quaternion__create(WORLDSPACE.up, up_rotation_theta);

  right_rotation_theta = atan(new_forward.z / new_forward.y);
  if (new_forward.y > 0) right_rotation_theta += M_PI;
  right_rotation= quaternion__create(WORLDSPACE.right, right_rotation_theta);

  bb->transform._rotation = quaternion__multiply(right_rotation, up_rotation);

  static struct m4x4 local_to_world;
  space__create_model(
    &WORLDSPACE,
    &bb->transform,
    &local_to_world
  );

  gpu->select_shader(bb->shader);
  gpu->select_texture(bb->texture);
  gpu->set_shader_m4x4(bb->shader, "model", &local_to_world);
  gpu->set_shader_m4x4(bb->shader, "view", &cam->lookat);
  gpu->set_shader_m4x4(bb->shader, "projection", &cam->projection);
  gpu->draw_mesh(&QUAD);
}