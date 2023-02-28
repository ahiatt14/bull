#include <math.h>

#include "tail.h"

#include "ecs.h"

#include "water.h"

#include "assets.h"
#include "constants.h"

#include "ocean_surface_mesh.h"
#include "ocean_frag.h"
#include "ocean_vert.h"

#include "solid_color_frag.h"

static Shader shader;

void water__copy_assets_to_gpu(
  GPU const *const gpu
) {

  shader.vert_src = OCEAN_VERT_SRC;
  shader.frag_src = OCEAN_FRAG_SRC;
  gpu->copy_shader_to_gpu(&shader);

  gpu->copy_static_mesh_to_gpu(&OCEAN_SURFACE_MESH);
}

static void draw_water(
  struct GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  struct Entity const *const water
) {

  static Shader *shader;
  static M4x4 model;
  static M3x3 normals_model;

  shader = water->draw.shader;

  space__create_model(&WORLDSPACE, &water->transform, &model);
  space__create_normals_model(&model, &normals_model);
  gpu->set_shader_m4x4(shader, "model", &model);
  gpu->set_shader_m3x3(shader, "normals_model", &normals_model);

  gpu->set_shader_vec3(shader, "camera_offset", camera->position);

  gpu->set_shader_float(shader, "wavelength", 3);
  gpu->set_shader_float(shader, "steepness", 0.4f);
  gpu->set_shader_vec2(
    shader,
    "direction",
    vec2__normalize((Vec2){ -1, -1 })
  );

  gpu->draw_mesh(water->draw.mesh);
}

EntityId create_water(
  struct ECS *const ecs
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
    (struct Draw){
      .shader = &shader,
      .mesh = &OCEAN_SURFACE_MESH,
      .textures = WATER_TEXTURE,
      .draw = draw_water
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
    (struct Draw){
      .shader = &FLAT_TEXTURE_SHADER,
      .mesh = &QUAD,
      .textures = WATER_TEXTURE,
      .draw = ecs__draw_mesh
    },
    ecs
  );

  return waves;
}