#include "tail.h"

#include "ecs.h"

#include "tail_helpers.h"
#include "constants.h"

void ecs__draw_mesh(
  struct GameTime time,
  struct Camera const *const camera,
  struct GPU const *const gpu,
  struct Entity const *const entity
) {

  static struct M4x4 model;
  static struct M3x3 normals_model;
  static struct Shader *shader;

  shader = entity->draw.shader;

  space__create_model(&WORLDSPACE, &entity->transform, &model);
  gpu->set_shader_m4x4(shader, "model", &model);
  space__create_normals_model(&model, &normals_model);
  gpu->set_shader_m3x3(shader, "normals_model", &normals_model);

  gpu->draw_mesh(entity->draw.mesh);
}

void ecs__draw_billboard(
  struct GameTime time,
  struct Camera const *const camera,
  struct GPU const *const gpu,
  struct Entity const *const entity
) {

  static struct M4x4 model, rotation;
  static struct Shader *shader;

  shader = entity->draw.shader;

  m4x4__translation(entity->transform.position, &model);

  quaternion__to_m4x4(entity->transform.rotation, &rotation);

  gpu->set_shader_m4x4(shader, "model", &model);
  gpu->set_shader_m4x4(shader, "rotation", &rotation);
  gpu->set_shader_float(shader, "scale", entity->transform.scale);

  gpu->draw_points(&POINT);
}