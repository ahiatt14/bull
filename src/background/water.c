#include <math.h>

#include "tail.h"

#include "ecs.h"

#include "water.h"

#include "assets.h"
#include "constants.h"

#include "ocean_surface_mesh.h"
#include "ocean_frag.h"
#include "ocean_vert.h"
#include "default_vert.h"

#include "solid_color_frag.h"

static Shader waves_shader;
static Shader far_water_shader;

static DrawableMesh far_water_quad;

void water__copy_assets_to_gpu(
  GPU const *const gpu
) {

  far_water_quad = QUAD;
  mesh__tile_uvs(100, 100, &far_water_quad);
  gpu->copy_static_mesh_to_gpu(&far_water_quad);

  waves_shader.vert_src = OCEAN_VERT_SRC;
  waves_shader.frag_src = OCEAN_FRAG_SRC;
  gpu->copy_shader_to_gpu(&waves_shader);

  far_water_shader.vert_src = DEFAULT_VERT_SRC;
  far_water_shader.frag_src = OCEAN_FRAG_SRC;
  gpu->copy_shader_to_gpu(&far_water_shader);

  gpu->copy_static_mesh_to_gpu(&OCEAN_SURFACE_MESH);
}

static void draw_waves(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  Entity const *const waves
) {

  static Shader *shader;
  static M4x4 model;
  static M3x3 normals_model;

  shader = waves->draw.shader;

  space__create_model(&WORLDSPACE, &waves->transform, &model);
  space__create_normals_model(&model, &normals_model);
  gpu->set_shader_m4x4(shader, "model", &model);
  gpu->set_shader_m3x3(shader, "normals_model", &normals_model);

  gpu->set_shader_vec3(shader, "camera_offset", camera->position);

  // Vec3 camera_to_origin = vec3_minus_vec3(ORIGIN, camera->position);

  // gpu->set_shader_vec2(
  //   shader,
  //   "direction",
  //   vec2__normalize((Vec2){ camera_to_origin.x, camera_to_origin.z })
  // );

  gpu->set_shader_vec3(shader, "color", COLOR_OCEAN_BLUE);

  gpu->draw_mesh(waves->draw.mesh);
}

static void draw_far_water(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  Entity const *const water
) {

  static Shader *shader;
  static M4x4 model;

  shader = water->draw.shader;

  space__create_model(&WORLDSPACE, &water->transform, &model);

  gpu->set_shader_m4x4(shader, "model", &model);
  gpu->set_shader_vec3(shader, "color", COLOR_OCEAN_BLUE);

  gpu->draw_mesh(water->draw.mesh);
}

EntityId create_water(
  ECS *const ecs
) {
  
  EntityId waves = ecs__create_entity(ecs);

  ecs__add_transform(
    waves,
    (Transform){
      .rotation = (Quaternion){0},
      .position = (Vec3){ 0, 1, 0 },
      .scale = 1
    },
    ecs
  );
  ecs__add_draw(
    waves,
    (Draw){
      .shader = &waves_shader,
      .mesh = &OCEAN_SURFACE_MESH,
      .textures = WATER_TEXTURE,
      .draw = draw_waves
    },
    ecs
  );

  EntityId far_water = ecs__create_entity(ecs);

  ecs__add_transform(
    far_water,
    (Transform){
      .position = (Vec3){0},
      .rotation = (Quaternion){ (Vec3){0}, 0 },
      .scale = 300
    },
    ecs
  );
  ecs__add_draw(
    far_water,
    (Draw){
      .shader = &FLAT_TEXTURE_SHADER,
      .mesh = &far_water_quad,
      .textures = WATER_TEXTURE,
      .draw = draw_far_water
    },
    ecs
  );

  return waves;
}