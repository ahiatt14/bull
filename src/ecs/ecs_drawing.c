#include "tail.h"

#include "ecs.h"

#include "tail_helpers.h"

static struct M4x4 model;
static struct M3x3 normals_model;
static struct Shader *shader;

void ecs__draw_mesh(
  struct GameTime time,
  struct Camera const *const camera,
  struct GPU const *const gpu,
  EntityId id,
  struct ECS const *const ecs
) {

  shader = ecs->entities[id].draw.shader;

  space__create_model(&WORLDSPACE, &ecs->entities[id].transform, &model);

  gpu->set_shader_m4x4(shader, "view", &camera->lookat);
  gpu->set_shader_m4x4(shader, "model", &model);
  gpu->set_shader_m4x4(shader, "projection", &camera->projection);

  space__create_normals_model(&model, &normals_model);

  gpu->set_shader_m3x3(shader, "normals_model", &normals_model);

  gpu->draw_mesh(ecs->entities[id].draw.mesh);
}

void ecs__draw_billboard(
  struct GameTime time,
  struct Camera const *const camera,
  struct GPU const *const gpu,
  EntityId id,
  struct ECS const *const ecs
) {

  shader = ecs->entities[id].draw.shader;

  m4x4__translation(ecs->entities[id].transform.position, &model);
  gpu->set_shader_m4x4(shader, "model", &model);
  gpu->set_shader_m4x4(shader, "view", &camera->lookat);
  gpu->set_shader_m4x4(shader, "projection", &camera->projection);
  gpu->set_shader_float(shader, "scale", ecs->entities[id].transform.scale);

  gpu->draw_points(&POINT);
}