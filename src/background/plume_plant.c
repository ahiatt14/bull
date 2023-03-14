#include <stdio.h> // TODO: debugging

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

// LOCALS

static Shader steam_shader;
static Shader solid_material_shader;
static Shader mist_shader;

// HELPER DECS

static void destroy_mist(
  EntityId discharge,
  Seconds remainder,
  ECS *const ecs
);

static void draw_plume(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  Transform const *const total_transform,
  EntityId id,
  ECS const *const ecs
);

static void spawn_discharge_mist(
  EntityId discharges,
  Seconds remainder,
  ECS *const ecs
);

// PUBLIC API

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
  mist_shader.vert_src = BILLBOARD_VERT_SRC;
  mist_shader.geo_src = BILLBOARD_GEO_SRC;
  gpu->copy_shader_to_gpu(&mist_shader);

  gpu->copy_static_mesh_to_gpu(&COOLING_TOWER_MESH);
  gpu->copy_static_mesh_to_gpu(&TOWER_PIPES_MESH);
  gpu->copy_static_mesh_to_gpu(&TOWER_DISCHARGES_MESH);
}

static void draw_plume(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  Transform const *const total_transform,
  EntityId id,
  ECS const *const ecs
);

EntityId create_plume_plant(
  Vec3 position,
  ECS *const ecs
) {

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

  ecs__add_parent_relationship(cooling_tower, pipes, ecs);
  ecs__add_transform(
    pipes,
    (Transform){
      .position = (Vec3){0},
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

  ecs__add_parent_relationship(cooling_tower, discharges, ecs);
  ecs__add_transform(
    discharges,
    (Transform){
      .position = (Vec3){0},
      .scale = 150
    },
    ecs
  );
  ecs__add_uv_scroll(
    discharges,
    (ScrollUV){
      .total = (Vec2){0},
      .speed = (Vec2){ 0.05f, -0.2f }
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
  ecs__add_repeat(
    discharges,
    (Repeat){
      .age = 1,
      .interval = 1,
      .on_interval = spawn_discharge_mist
    },
    ecs
  );

  EntityId plume = ecs__create_entity(ecs);

  ecs__add_parent_relationship(cooling_tower, plume, ecs);
  ecs__add_transform(
    plume,
    (Transform){
      .scale = 150,
      .position = (Vec3){0}
    },
    ecs
  );
  ecs__add_uv_scroll(
    plume,
    (ScrollUV){
      .speed = (Vec2){ 0, -0.01f },
      .total = (Vec2){0}
    },
    ecs
  );
  ecs__add_draw(
    plume,
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

// HELPER DEFS

static void spawn_discharge_mist(
  EntityId discharges,
  Seconds remainder,
  ECS *const ecs
) {

  static Vec3 random_offsets[6] = {
    { 20, 0, 10 },
    { 0, 0, 30 },
    { 0, 0, 0 },
    { -20, 0, 20 },
    { 10, 0, -10 },
    { 15, 0, 0 }
  };
  static uint_fast8_t random_offset_i = 0;

  EntityId mist = ecs__create_entity(ecs);

  Vec3 initial_position = (Vec3){
    0,
    10,
    2.45f * ecs->entities[discharges].transform.scale
  };

  random_offset_i = (random_offset_i + 1) % 6;

  ecs__add_alpha_effect(mist, ecs);
  ecs__add_transform(
    mist,
    (Transform){
      .position = vec3_plus_vec3(
        initial_position,
        random_offsets[random_offset_i]
      ),
      .scale = 20,
      .rotation = quaternion__create(
        WORLDSPACE.forward,
        vec3__magnitude(random_offsets[random_offset_i])
      )
    },
    ecs
  );
  ecs__add_velocity(
    mist,
    (Vec3){ 0, 5, 0 },
    ecs
  );
  ecs__add_draw(
    mist,
    (Draw){
      .draw = ecs__draw_billboard,
      .textures = MIST_TEXTURE,
      .shader = &mist_shader,
      .mesh = NULL
    },
    ecs
  );
  ecs__add_timeout(
    mist,
    (Timeout){
      .age = remainder,
      .limit = 8,
      .on_timeout = destroy_mist
    },
    ecs
  );
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

static void destroy_mist(
  EntityId mist,
  Seconds remainder,
  ECS *const ecs
) {
  ecs__mark_for_destruction(mist, ecs);
}