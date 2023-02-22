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

static struct Shader shader;

void water__copy_assets_to_gpu(
  struct GPU const *const gpu
) {

  shader.vert_src = OCEAN_VERT_SRC;
  shader.frag_src = OCEAN_FRAG_SRC;
  gpu->copy_shader_to_gpu(&shader);

  gpu->copy_static_mesh_to_gpu(&OCEAN_SURFACE_MESH);
}

static void draw_water(
  struct GameTime time,
  struct Camera const *const camera,
  struct GPU const *const gpu,
  struct Entity const *const water
) {

  static struct Shader *shader;
  static struct M4x4 model;
  static struct M3x3 normals_model;

  shader = water->draw.shader;

  space__create_model(&WORLDSPACE, &water->transform, &model);
  space__create_normals_model(&model, &normals_model);
  gpu->set_shader_m4x4(shader, "model", &model);
  gpu->set_shader_m3x3(shader, "normals_model", &normals_model);

  gpu->set_shader_vec3(shader, "cam_world_pos", camera->position);

  gpu->set_shader_float(shader, "wavelength", 3);
  gpu->set_shader_float(shader, "steepness", 0.4f);
  gpu->set_shader_vec2(
    shader,
    "direction",
    vec2__normalize((struct Vec2){ -1, -1 })
  );

  gpu->draw_mesh(water->draw.mesh);
}

void create_water(
  struct ECS *const ecs
) {
  
  EntityId near_water = ecs__create_entity(ecs);

  ecs__add_transform(
    near_water,
    (struct Transform){
      .rotation = (struct Quaternion){0},
      .position = (struct Vec3){ 0, 1, 0 },
      .scale = 1
    },
    ecs
  );
  ecs__add_draw(
    near_water,
    (struct Draw){
      .shader = &shader,
      .mesh = &OCEAN_SURFACE_MESH,
      .texture = TEXTURES[WATER_TEXTURE],
      .draw = draw_water
    },
    ecs
  );

  EntityId far_water = ecs__create_entity(ecs);

  ecs__add_transform(
    far_water,
    (struct Transform){
      .position = (struct Vec3){0},
      .rotation = quaternion__create(WORLDSPACE.right, -M_PI * 0.5f),
      .scale = 200
    },
    ecs
  );
  ecs__add_draw(
    far_water,
    (struct Draw){
      .shader = &FLAT_TEXTURE_SHADER,
      .mesh = &QUAD,
      .texture = TEXTURES[WATER_TEXTURE],
      .draw = ecs__draw_mesh
    },
    ecs
  );
}