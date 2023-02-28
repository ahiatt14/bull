#include <stdio.h>

#include "tail.h"

#include "ecs.h"

#include "assets.h"
#include "tail_helpers.h"
#include "constants.h"

// TODO: arbitrary for now
#define MAX_TEXTURES_PER_ENTITY 3

static inline uint_fast8_t has_texture(
  uint_fast16_t texture,
  uint_fast16_t entity_texture_mask
) {
  uint_fast16_t anded = texture & entity_texture_mask;
  return (anded == texture) ? 1 : 0;
}

// TODO: expensive thing to do for every draw call
void set_textures(
  Entity const *const entity,
  GPU const *const gpu
) {

  uint_fast8_t selected_texture_count = 0;

  static Texture const *textures[MAX_TEXTURES_PER_ENTITY];

  for (uint_fast16_t i = 0; i < TEXTURE_COUNT; i++)
    if (has_texture(1 << i, entity->draw.textures))
      textures[selected_texture_count++] = TEXTURES[i];

  if (selected_texture_count > 1) {
    gpu->select_textures(
      entity->draw.shader,
      selected_texture_count,
      textures
    );
  } else {
    gpu->select_texture(textures[0]);
  }
}

void ecs__draw_mesh(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  Entity const *const entity
) {

  static M4x4 model;
  static M3x3 normals_model;
  static Shader *shader;

  shader = entity->draw.shader;

  space__create_model(&WORLDSPACE, &entity->transform, &model);
  gpu->set_shader_m4x4(shader, "model", &model);
  space__create_normals_model(&model, &normals_model);
  gpu->set_shader_m3x3(shader, "normals_model", &normals_model);

  gpu->draw_mesh(entity->draw.mesh);
}

void ecs__draw_billboard(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  Entity const *const entity
) {

  static M4x4 model, rotation;
  static Shader *shader;

  shader = entity->draw.shader;

  m4x4__translation(entity->transform.position, &model);

  quaternion__to_m4x4(entity->transform.rotation, &rotation);

  gpu->set_shader_m4x4(shader, "model", &model);
  gpu->set_shader_m4x4(shader, "rotation", &rotation);
  gpu->set_shader_float(shader, "scale", entity->transform.scale);

  gpu->draw_points(&POINT);
}