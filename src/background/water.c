#include <math.h>
#include <stdio.h>

#include "tail.h"

#include "ecs.h"

#include "water.h"

#include "assets.h"
#include "constants.h"

#include "tess_vert.h"
#include "waves_ctrl.h"
#include "waves_eval.h"
#include "waves_frag.h"

#include "normal_debug_frag.h"

#define TESS_QUAD_VERTS_PER_EDGE 32
#define TESS_QUAD_VERT_COUNT 1024
#define TESS_QUAD_INDEX_COUNT 10000

#define WAVE_COUNT 3

typedef struct BULLWAVE {
  float wavelength;
  float steepness;
  Vec2 direction;
} Wave;

DrawableMesh tess_quad = (DrawableMesh){
  .vertices = (Vertex[TESS_QUAD_VERT_COUNT]){0},
  .vertices_length = TESS_QUAD_VERT_COUNT,
  .vertices_size = sizeof(Vertex) * TESS_QUAD_VERT_COUNT,
  .indices = (unsigned int[TESS_QUAD_INDEX_COUNT]){0},
  .indices_length = TESS_QUAD_INDEX_COUNT,
  .indices_size = sizeof(unsigned int) * TESS_QUAD_INDEX_COUNT
};

void fill_tess_quad_data(
  float patch_length,
  uint8_t verts_per_edge,
  DrawableMesh *const mesh
);

static void draw_waves(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  Transform const *const hierarchy_transform,
  EntityId id,
  ECS const *const ecs
);

static Shader waves_shader;

void water__copy_assets_to_gpu(
  GPU const *const gpu
) {

  waves_shader.vert_src = TESS_VERT_SRC;
  waves_shader.tess_ctrl_src = WAVES_CTRL_SRC;
  waves_shader.tess_eval_src = WAVES_EVAL_SRC;
  waves_shader.frag_src = WAVES_FRAG_SRC;
  gpu->copy_shader_to_gpu(&waves_shader);

  fill_tess_quad_data(100, TESS_QUAD_VERTS_PER_EDGE, &tess_quad);
  // TODO: move to loading.c
  gpu->copy_tessellated_mesh_to_gpu(&tess_quad);
}

EntityId create_water(
  ECS *const ecs
) {
  
  EntityId waves = ecs__create_entity(ecs);

  ecs__add_transform(
    waves,
    (Transform){
      .rotation = (Quaternion){0},
      .position = (Vec3){0},
      .scale = 1
    },
    ecs
  );
  // TODO: shouldn't be necessary fml, check the winding in
  // waves_eval.glsl
  ecs__add_draw_back_faces(waves, ecs);
  ecs__add_receives_light(waves, ecs);
  ecs__add_uv_scroll(
    waves,
    (ScrollUV){
      .total = (Vec2){0},
      .speed = (Vec2){ 0, 100 }
    },
    ecs
  );
  ecs__add_draw(
    waves,
    (Draw){
      .shader = &waves_shader,
      .mesh = &tess_quad,
      .textures = WATER_TEXTURE | WAVE_CREST_TEXTURE,
      .draw = draw_waves
    },
    ecs
  );

  return waves;
}

void fill_tess_quad_data(
  float patch_length,
  uint8_t verts_per_edge,
  DrawableMesh *const mesh
) {

  int patches_per_edge = verts_per_edge - 1;
  float quad_edge_length = patch_length * patches_per_edge;

  float origin_offset = quad_edge_length * 0.5;

  int index_count = 0;
  for (int vert_row = 0; vert_row < verts_per_edge; vert_row++)
  for (int vert_col = 0; vert_col < verts_per_edge; vert_col++) {

    int i = vert_col + vert_row * verts_per_edge;

    mesh->vertices[i] = (Vertex){
      .position = (Vec3){
        vert_col * patch_length - origin_offset,
        0,
        vert_row * patch_length - origin_offset
      },
      .normal = (Vec3){ 0, 1, 0 },
      // .uv = (Vec2){ vert_row, vert_col } // NOTE: this tiles a texture per patch
      .uv = (Vec2){ vert_row * 5, vert_col * 5 }
    };

    if (
      vert_row == verts_per_edge - 1 ||
      vert_col == verts_per_edge - 1
    ) continue;

    mesh->indices[index_count++] = i;
    mesh->indices[index_count++] = i + 1;
    mesh->indices[index_count++] = i + 1 + verts_per_edge;
    mesh->indices[index_count++] = i + verts_per_edge;
  }
}

static void draw_waves(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  Transform const *const hierarchy_transform,
  EntityId id,
  ECS const *const ecs
) {

  static Entity const *water;
  water = &ecs->entities[id];

  static Shader *shader;
  static M4x4 model;
  static M3x3 normals_model;

  shader = water->draw.shader;

  space__create_model(&WORLDSPACE, hierarchy_transform, &model);
  space__create_normals_model(&model, &normals_model);
  gpu->set_shader_m4x4(shader, "model", &model);
  gpu->set_shader_m3x3(shader, "normals_model", &normals_model);

  gpu->set_shader_int(shader, "max_tess", 16);
  gpu->set_shader_float(shader, "min_dist", 200);
  gpu->set_shader_float(shader, "max_dist", 500);

  gpu->set_shader_float(shader, "gravity", 5);

  gpu->set_shader_float(shader, "min_altitude", 1);
  gpu->set_shader_float(shader, "max_altitude", 4);

  static Wave waves[WAVE_COUNT] = {
    { 20, 0.15f, (Vec2){ 1, 0 } },
    { 25, 0.3f, (Vec2){ 0, 1 } },
    { 40, 0.12f, (Vec2){ .707f, -.707f } }
  };

  gpu->set_shader_int(shader, "wave_count", WAVE_COUNT);
  static char uniform_name[40];
  for (uint_fast8_t i = 0; i < WAVE_COUNT; i++) {
    sprintf(uniform_name, "waves[%u].wavelength", i);
    gpu->set_shader_float(shader, uniform_name, waves[i].wavelength);
    sprintf(uniform_name, "waves[%u].steepness", i);
    gpu->set_shader_float(shader, uniform_name, waves[i].steepness);
    sprintf(uniform_name, "waves[%u].direction", i);
    gpu->set_shader_vec2(shader, uniform_name, waves[i].direction);
  }

  gpu->draw_tessellated_mesh(water->draw.mesh);
}