#include "tail.h"

#include "ecs.h"

#include "plume_plant.h"

#include "constants.h"
#include "assets.h"
#include "bull_math.h"


#include "default_vert.h"
#include "billboard_geo.h"
#include "billboard_vert.h"

#include "flat_texture_frag.h"
#include "steam_column_mesh.h"
#include "steam_frag.h"
#include "steam_geo.h"

#include "mountain_frag.h"

#include "mist_frag.h"

#include "cooling_tower_mesh.h"
#include "tower_pipes_mesh.h"
#include "tower_discharges_mesh.h"
#include "tower_discharge_mists_mesh.h"

static Shader steam_shader;
// static Shader discharge_shader;
static Shader solid_material_shader;
static Shader mist_shader;

void plume_plant__copy_assets_to_gpu(
  GPU const *const gpu
) {

  steam_shader.frag_src = FLAT_TEXTURE_FRAG_SRC;
  steam_shader.geo_src = STEAM_GEO_SRC;
  steam_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&steam_shader);
  gpu->copy_dynamic_mesh_to_gpu(&STEAM_COLUMN_MESH);

  solid_material_shader.frag_src = MOUNTAIN_FRAG_SRC;
  solid_material_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&solid_material_shader);

  mist_shader.frag_src = MIST_FRAG_SRC;
  mist_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&mist_shader);

  gpu->copy_static_mesh_to_gpu(&COOLING_TOWER_MESH);
  gpu->copy_static_mesh_to_gpu(&TOWER_PIPES_MESH);
  gpu->copy_static_mesh_to_gpu(&TOWER_DISCHARGES_MESH);
  gpu->copy_static_mesh_to_gpu(&TOWER_DISCHARGE_MISTS_MESH);
}

static void draw_plume(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  Transform const *const total_transform,
  EntityId id,
  ECS const *const ecs
) {

  static Entity const *plume;
  plume = &ecs->entities[id];

  static Shader *shader;
  static M4x4 model;
  // static M3x3 normals_model;

  shader = plume->draw.shader;

  space__create_model(&WORLDSPACE, total_transform, &model);
  // space__create_normals_model(&model, &normals_model);
  gpu->set_shader_m4x4(shader, "model", &model);
  // gpu->set_shader_m3x3(shader, "normals_model", &normals_model);

  gpu->set_shader_float(shader, "wavelength", 10);
  gpu->set_shader_float(shader, "amplitude", 0.2);
  gpu->set_shader_float(shader, "speed", -0.01f);

  gpu->draw_mesh(plume->draw.mesh);
}

EntityId create_plume_plant(
  Vec3 position,
  ECS *const ecs
) {

    // COOLING TOWER

  EntityId cooling_tower = ecs__create_entity(ecs);

  ecs__add_transform(
    cooling_tower,
    (Transform){
      .position = position,
      .scale = 150
    },
    ecs
  );
  ecs__add_draw(
    cooling_tower,
    (Draw){
      .mesh = &COOLING_TOWER_MESH,
      .textures = CONCRETE_WALL_TEXTURE,
      .shader = &solid_material_shader,
      .draw = ecs__draw_mesh
    },
    ecs
  );
  
  EntityId pipes = ecs__create_entity(ecs);

  ecs__add_transform(
    pipes,
    (Transform){
      .position = position,
      .scale = 150
    },
    ecs
  );
  ecs__add_draw(
    pipes,
    (Draw){
      .mesh = &TOWER_PIPES_MESH,
      .textures = DARK_RUST_TEXTURE,
      .shader = &solid_material_shader,
      .draw = ecs__draw_mesh
    },
    ecs
  );

  EntityId discharges = ecs__create_entity(ecs);

  ecs__add_transform(
    discharges,
    (Transform){
      .position = position,
      .scale = 150
    },
    ecs
  );
  ecs__add_uv_scroll(
    discharges,
    (ScrollUV){
      .total = (Vec2){0},
      .speed = (Vec2){ 0, 0.2f }
    },
    ecs
  );
  ecs__add_draw(
    discharges,
    (Draw){
      .mesh = &TOWER_DISCHARGES_MESH,
      .textures = WATER_TEXTURE,
      .shader = &solid_material_shader,
      .draw = ecs__draw_mesh
    },
    ecs
  );

  EntityId mists = ecs__create_entity(ecs);

  ecs__add_transform(
    mists,
    (Transform){
      .position = position,
      .scale = 150
    },
    ecs
  );
  ecs__add_uv_scroll(
    mists,
    (ScrollUV){
      .total = (Vec2){0},
      .speed = (Vec2){ 0, -0.03f }
    },
    ecs
  );
  ecs__add_alpha_effect(mists, ecs);
  ecs__add_draw(
    mists,
    (Draw){
      .mesh = &TOWER_DISCHARGE_MISTS_MESH,
      .textures = MIST_TEXTURE,
      .shader = &mist_shader,
      .draw = ecs__draw_mesh
    },
    ecs
  );

  // LIGHTS

  // EntityId light = ecs__create_entity(ecs);

  // ecs__add_transform(
  //   light,
  //   (Transform){
  //     .position = (Vec3){ 40, 2, 0 },
  //     .scale = 10
  //   },
  //   ecs
  // );
  // ecs__add_draw(
  //   light,
  //   (Draw){
  //     .draw = ecs__draw_billboard,
  //     .shader = &FLAT_TEXTURE_SHADER,
  //     .textures = COOLING_TOWER_LIGHT_TEXTURE
  //   },
  //   ecs
  // );

  // STEAM

  EntityId steam = ecs__create_entity(ecs);

  ecs__add_transform(
    steam,
    (Transform){
      .scale = 150,
      .position = position
    },
    ecs
  );
  ecs__add_uv_scroll(
    steam,
    (ScrollUV){
      .speed = (Vec2){ 0, -0.01f },
      .total = (Vec2){0}
    },
    ecs
  );
  ecs__add_draw(
    steam,
    (Draw){
      .textures = STEAM_TEXTURE,
      .draw = draw_plume,
      .shader = &steam_shader,
      .mesh = &STEAM_COLUMN_MESH
    },
    ecs
  );

  return cooling_tower;
}