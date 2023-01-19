#include "tail.h"

#include "billboard.h"

#include "constants.h"
#include "bull_math.h"

void billboard__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct billboard *const bb
) {

  static struct vec3 forward, right, up; 
  static struct m4x4 lookat, translation, local_to_world;

  bb->transform.rotation_in_deg.y += 0.05f;

  space__create_model(
    &WORLDSPACE,
    &bb->transform,
    &local_to_world
  );

  // forward = vec3__normalize(vec3_minus_vec3(
  //   cam->position,
  //   bb->transform.position
  // ));
  // right = vec3__normalize(vec3__cross(
  //   WORLDSPACE.up,
  //   forward
  // ));
  // up = vec3__normalize(vec3__cross(
  //   forward, right
  // ));

  // m4x4__view(right, up, forward, &lookat);
  // m4x4__translation(bb->transform.position, &translation);
  // // m4x4__scaling(t->scale, &scale);

  // m4x4_x_m4x4(
  //   &lookat,
  //   &translation,
  //   &local_to_world
  // );

  gpu->select_shader(bb->shader);
  gpu->select_texture(bb->texture);
  gpu->set_shader_m4x4(bb->shader, "model", &local_to_world);
  gpu->set_shader_m4x4(bb->shader, "view", &cam->lookat);
  gpu->set_shader_m4x4(bb->shader, "projection", &cam->projection);
  gpu->draw_mesh(&QUAD);
}