#include <stdio.h>

#include "tail.h"

#include "ecs.h"

#include "assets.h"
#include "tail_helpers.h"
#include "constants.h"

#define MAX_TEXTURES_PER_ENTITY 5

// TODO: lots of optimization and cleanup possible in here

// HELPER DECS

// TODO: will want to move eventually once
// other systems take advantage of hierarchies
static void add_parents_transforms(
  Transform *const total_transform,
  Entity const *const child,
  ECS const *const ecs
);

static inline uint_fast8_t has_texture(
  uint_fast16_t texture,
  uint_fast16_t entity_texture_mask
);

static void set_textures(
  Entity const *const entity,
  GPU const *const gpu
);

// PUBLIC API

void ecs__draw_mesh(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  Transform const *const total_transform,
  EntityId id,
  ECS const *const ecs
) {

  static Entity const *entity;
  entity = &ecs->entities[id];

  static M4x4 model;
  static M3x3 normals_model;
  static Shader *shader;

  shader = entity->draw.shader;

  space__create_model(&WORLDSPACE, total_transform, &model);
  gpu->set_shader_m4x4(shader, "model", &model);
  space__create_normals_model(&model, &normals_model);
  gpu->set_shader_m3x3(shader, "normals_model", &normals_model);

  gpu->draw_mesh(entity->draw.mesh);
}

void ecs__draw_billboard(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  Transform const *const total_transform,
  EntityId id,
  ECS const *const ecs
) {

  static Entity const *entity;
  entity = &ecs->entities[id];
  static M4x4 model, rotation;
  static Shader *shader;

  shader = entity->draw.shader;

  m4x4__translation(total_transform->position, &model);

  quaternion__to_m4x4(total_transform->rotation, &rotation);

  gpu->set_shader_m4x4(shader, "model", &model);
  gpu->set_shader_m4x4(shader, "rotation", &rotation);
  gpu->set_shader_float(shader, "scale", total_transform->scale);

  gpu->draw_points(&POINT);
}

void ecs__prepare_entity_draw(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  EntityId id,
  ECS const *const ecs
) {

  static Entity const *entity;
  entity = &ecs->entities[id];

  Shader *shader = entity->draw.shader;

  gpu->select_shader(shader);

  if (entity->draw.textures > 0) set_textures(entity, gpu);

  gpu->set_shader_float(
    shader,
    "total_elapsed_seconds",
    time.sec_since_game_launch
  );

  // TODO: mmmm idk bout this
  // it seems like uniform values are being preserved between
  // invokations of the *same* shader regardless of whether
  // that uniform is set. investigate
  has_component(c_UV_SCROLL, entity->config) ?
  gpu->set_shader_vec2(shader, "uv_scroll", entity->scroll_uv.total) :
  gpu->set_shader_vec2(shader, "uv_scroll", (Vec2){0});

  if (has_component(c_TIMEOUT, entity->config)) {
    gpu->set_shader_float(
      shader,
      "seconds_since_activation",
      entity->timeout.age
    );
    gpu->set_shader_float(
      shader,
      "limit_in_seconds",
      entity->timeout.limit
    );
  }

  gpu->set_shader_m4x4(shader, "view", &camera->lookat);
  gpu->set_shader_m4x4(shader, "projection", &camera->projection);

  static Transform total_transform;
  total_transform = entity->transform;
  
  if (has_component(
    c_HAS_PARENT,
    entity->config
  )) add_parents_transforms(&total_transform, entity, ecs);

  if (has_component(c_DRAW_BACK_FACES, entity->config))
    gpu->cull_no_faces();

  entity->draw.draw(
    time,
    camera,
    gpu,
    &total_transform,
    id,
    ecs
  );

  gpu->cull_back_faces();
}

static inline void swap(EntityId *id0, EntityId *id1) {
  EntityId temp = *id0; *id0 = *id1; *id1 = temp;
}
void sort_alpha_entities(
  Vec3 camera_position,
  EntityId *alpha_entities,
  uint_fast16_t alpha_entity_count,
  ECS const *const ecs
) {

  int i, j, min_i;

  float j_y, min_i_y;

  for (i = 0; i < alpha_entity_count - 1; i++) {

    min_i = i;

    for (j = i + 1; j < alpha_entity_count; j++) {
      
      j_y = ecs->entities[alpha_entities[j]].transform.position.y;
      min_i_y = ecs->entities[alpha_entities[min_i]].transform.position.y;

      if (j_y < min_i_y) min_i = j;
    }

    swap(&alpha_entities[min_i], &alpha_entities[i]);
  }
}

// HELPER DEFS

static void add_parents_transforms(
  Transform *const total_transform,
  Entity const *const child,
  ECS const *const ecs
) {

  static Entity const *parent;
  parent = &ecs->entities[child->hierarchy.parent];

  total_transform->position = space__ccw_quat_rotate(
    parent->transform.rotation,
    total_transform->position
  );
  total_transform->position = vec3_plus_vec3(
    total_transform->position,
    parent->transform.position
  );
  total_transform->rotation = quaternion__multiply(
    total_transform->rotation,
    parent->transform.rotation
  );

  if (has_component(c_HAS_PARENT, parent->config))
    add_parents_transforms(total_transform, parent, ecs);
}

static inline uint_fast8_t has_texture(
  uint_fast16_t texture,
  uint_fast16_t entity_texture_mask
) {
  uint_fast16_t anded = texture & entity_texture_mask;
  return (anded == texture) ? 1 : 0;
}

static void set_textures(
  Entity const *const entity,
  GPU const *const gpu
) {

  // TODO: expensive thing to do for every draw call

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