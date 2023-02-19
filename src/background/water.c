#include "tail.h"

#include "ecs.h"

#include "constants.h"

#include "water.h"

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

  gpu->set_shader_float(shader, "wavelength", 0.1f);
  gpu->set_shader_float(shader, "steepness", 0.3f);

  gpu->draw_mesh(water->draw.mesh);
}

void create_water(
  struct ECS *const ecs
) {
  
  EntityId water = ecs__create_entity(ecs);

  ecs__add_transform(
    water,
    (struct Transform){
      .position = vec3_minus_vec3(
        ORIGIN,
        (struct Vec3){ 0, 1, 0 }
      ),
      .scale = 10
    },
    ecs
  );
  ecs__add_draw(
    water,
    (struct Draw){
      .shader = &shader,
      .mesh = &OCEAN_SURFACE_MESH,
      .texture = NULL,
      .draw = draw_water
    },
    ecs
  );
}