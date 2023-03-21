#include <math.h>
#include <stdio.h>

#include "tail.h"

#include "ecs.h"

#include "lighting.h"
#include "assets.h"
#include "tail_helpers.h"
#include "constants.h"
#include "bull_math.h"

#define MAX_TEXTURES_PER_ENTITY 5

// move this to lighting.c
#define ATTENUATION_COUNT 12
// TODO: maybe look for a function that can approximate a LUT like this?
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

static Vec2 calculate_attenuation(
  float strength
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

  // TODO: stash this state in an entity
  gpu->set_shader_vec3(shader, "ambient_color", COLOR_WHITE);
  gpu->set_shader_float(shader, "ambient_strength", 0.2f);

  Entity const *light_source;
  static char uniform_name[40];
  for (uint_fast8_t i = 0; i < lighting->point_light_count; i++) {

    light_source = &ecs->entities[lighting->point_lights[i]];

    // TODO: adjust light strength with timeout ratio??? good for blinks, etc

    // printf(
    //   "attenuation: linear: %.6f quadratic %.6f\n",
    //   calculate_attenuation(light_source->point_light.strength).x,
    //   calculate_attenuation(light_source->point_light.strength).y
    // );

    gpu->set_shader_int(
      shader,
      "point_light_count",
      lighting->point_light_count
    );
    sprintf(uniform_name, "point_lights[%i].position", i);
    gpu->set_shader_vec3(
      shader,
      uniform_name,
      light_source->transform.position
    );
    sprintf(uniform_name, "point_lights[%i].color", i);
    gpu->set_shader_vec3(
      shader,
      uniform_name,
      light_source->point_light.color
    );
    sprintf(uniform_name, "point_lights[%i].attenuation", i);
    gpu->set_shader_vec2(
      shader,
      uniform_name,
      calculate_attenuation(light_source->point_light.strength)
    );
  }

  gpu->set_shader_m4x4(shader, "view", &camera->lookat);
  gpu->set_shader_m4x4(shader, "projection", &camera->projection);

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

static void add_parents_transforms(
  Transform *const total_transform,
  Entity const *const child,
  ECS const *const ecs
) {

  static Entity const *parent;
  parent = &ecs->entities[child->hierarchy.parent];

  total_transform->position = space__ccw_quat_rotate(
    parent->transform.rotation,
    scalar_x_vec3(
      parent->transform.scale,
      total_transform->position
    )
  );
  total_transform->position = vec3_plus_vec3(
    total_transform->position,
    parent->transform.position
  );
  total_transform->scale *= parent->transform.scale;
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

static inline uint_fast8_t closest_to(float min, float max, float value) {
  return fabs(value - min) < fabs(value - max) ? 0 : 1;
}
// static Vec2 calculate_attenuation(
//   float strength
// ) {

//   uint_fast8_t attenuation_index = 0;

//   while (attenuation_index < ATTENUATION_COUNT - 1) {

//     if (strength >= LIGHT_ATTENUATIONS[attenuation_index + 1].x) {
//       attenuation_index++;
//       continue;
//     }

//     attenuation_index = closest_to(
//       LIGHT_ATTENUATIONS[attenuation_index].x,
//       LIGHT_ATTENUATIONS[attenuation_index + 1].x,
//       strength
//     );
//     break;
//   }

//   return (Vec2){
//     LIGHT_ATTENUATIONS[attenuation_index].y,
//     LIGHT_ATTENUATIONS[attenuation_index].z
//   };
// }
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