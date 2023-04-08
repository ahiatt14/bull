#include <math.h>
#include <stdio.h>

#include "tail.h"

#include "ecs.h"

#include "hierarchies.h"
#include "lighting.h"
#include "assets.h"
#include "tail_helpers.h"
#include "constants.h"
#include "bull_math.h"

#define MAX_TEXTURES_PER_ENTITY 5

// TODO: move this to lighting.c
#define ATTENUATION_COUNT 12

const Vec3 LIGHT_ATTENUATIONS[ATTENUATION_COUNT] = {
  { 7, 0.7f, 1.8f },
  { 13, 0.35f, 0.44f },
  { 20, 0.22f, 0.20f },
  { 32, 0.14f, 0.07f },
  { 50, 0.09f, 0.032f },
  { 65, 0.07f, 0.017f },
  { 100, 0.045f, 0.0075f },
  { 160, 0.027f, 0.0028f },
  { 200, 0.022f, 0.0019f },
  { 325, 0.014f, 0.0007f },
  { 600, 0.007, 0.0002f },
  { 3250, 0.0014, 0.000007f }
};

// TODO: lots of optimization and cleanup possible in here

// HELPER DECS

void light_entity(
  Lighting const *const lighting,
  GPU const *const gpu,
  EntityId id,
  ECS const *const ecs
);

static Vec2 calculate_attenuation(
  float strength
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
  Lighting const *const lighting,
  EntityId id,
  ECS const *const ecs
) {

  static Entity const *entity;
  entity = &ecs->entities[id];

  Shader *shader = entity->draw.shader;

  gpu->select_shader(shader);

  gpu->set_shader_m4x4(shader, "view", &camera->lookat);
  gpu->set_shader_m4x4(shader, "projection", &camera->projection);

  static Transform total_transform;
  total_transform = entity->transform;
  
  if (has_component(
    c_HAS_PARENT,
    entity->config
  )) add_parents_transforms(&total_transform, entity, ecs);

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

  // TODO: could send this as a single ratio float?
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

  if (has_component(
    c_RECEIVES_LIGHT,
    entity->config
  )) light_entity(lighting, gpu, id, ecs);

  if (has_component(c_DRAW_BACK_FACES, entity->config))
    gpu->cull_no_faces();

  entity->draw.draw(time, camera, gpu, &total_transform, id, ecs);

  gpu->cull_back_faces();
}

// HELPER DEFS

static inline void swap(EntityId *id0, EntityId *id1) {
  EntityId temp = *id0; *id0 = *id1; *id1 = temp;
}
void sort_alpha_entities(
  Vec3 camera_position,
  EntityId *alpha_entities,
  uint_fast16_t alpha_entity_count,
  ECS const *const ecs
) {

  int i, j, max_i;

  float j_distance, max_i_distance;

  for (i = 0; i < alpha_entity_count - 1; i++) {

    max_i = i;

    for (j = i + 1; j < alpha_entity_count; j++) {
      
      j_distance = vec3__distance(
        ecs->entities[alpha_entities[j]].transform.position,
        camera_position
      );
      max_i_distance = vec3__distance(
        ecs->entities[alpha_entities[max_i]].transform.position,
        camera_position
      );

      if (j_distance > max_i_distance) max_i = j;
    }

    swap(&alpha_entities[max_i], &alpha_entities[i]);
  }
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

static Vec2 calculate_attenuation(
  float strength
) {

  uint_fast8_t i = 0;

  for (i = 0; i < ATTENUATION_COUNT - 2; i++)
    if (strength < LIGHT_ATTENUATIONS[i + 1].x) break;

  float ratio =
    (strength - LIGHT_ATTENUATIONS[i].x) /
    (LIGHT_ATTENUATIONS[i + 1].x - LIGHT_ATTENUATIONS[i].x);
  
  return (Vec2){
    flerp(LIGHT_ATTENUATIONS[i].y, LIGHT_ATTENUATIONS[i+1].y, ratio),
    flerp(LIGHT_ATTENUATIONS[i].z, LIGHT_ATTENUATIONS[i+1].z, ratio)
  };
}

void light_entity(
  Lighting const *const lighting,
  GPU const *const gpu,
  EntityId id,
  ECS const *const ecs
) {

  static Shader const *shader;
  shader = ecs->entities[id].draw.shader;

  // TODO: can I send a whole struct at once?
  // UBO!
  gpu->set_shader_vec3(
    shader,
    "ambient_color", 
    lighting->ambient.color
  );
  gpu->set_shader_float(
    shader,
    "ambient_strength",
    lighting->ambient.strength
  );

  gpu->set_shader_vec3(
    shader,
    "skylight_direction",
    lighting->sky.direction
  );
  gpu->set_shader_vec3(
    shader,
    "skylight_color",
    lighting->sky.color
  );
  gpu->set_shader_float(
    shader,
    "skylight_strength",
    lighting->sky.strength
  );

  // TODO: optimize with UBO?
  static Entity const *point_source;
  static char uniform_name[40];
  Transform point_light_hierarchy_transform;
  for (uint_fast8_t i = 0; i < lighting->point_count; i++) {

    if (lighting->point_sources[i] == id) continue;

    point_source = &ecs->entities[lighting->point_sources[i]];
    point_light_hierarchy_transform = point_source->transform;

    // TODO: adjust light strength with timeout ratio??? good for blinks, etc

    if (has_component(
      c_HAS_PARENT,
      point_source->config
    )) add_parents_transforms(
      &point_light_hierarchy_transform,
      point_source,
      ecs
    );

    printf(
      "ct: %u pos: %.2f %.2f %.2f col: %.2f %.2f %.2f att: %.2f %.2f\n",
      lighting->point_count,
      point_light_hierarchy_transform.position.x,
      point_light_hierarchy_transform.position.y,
      point_light_hierarchy_transform.position.z,
      point_source->point_light.color.x,
      point_source->point_light.color.y,
      point_source->point_light.color.z,
      calculate_attenuation(point_source->point_light.strength).x,
      calculate_attenuation(point_source->point_light.strength).y
    );
    
    // TODO: possible debug opportunity for lingering point light issue
    // try sending 0 before light_entity is called if there's 0 lights???
    gpu->set_shader_int(
      shader,
      "point_count",
      lighting->point_count
    );
    sprintf(uniform_name, "point_lights[%i].position", i);
    gpu->set_shader_vec3(
      shader,
      uniform_name,
      point_light_hierarchy_transform.position
    );
    sprintf(uniform_name, "point_lights[%i].color", i);
    gpu->set_shader_vec3(
      shader,
      uniform_name,
      point_source->point_light.color
    );
    sprintf(uniform_name, "point_lights[%i].attenuation", i);
    gpu->set_shader_vec2(
      shader,
      uniform_name,
      calculate_attenuation(point_source->point_light.strength)
    );
  }
}