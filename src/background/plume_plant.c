#include <string.h>
#include <math.h>

#include "tail.h"

#include "ecs.h"

#include "plume_plant.h"

#include "colors.h"
#include "constants.h"
#include "assets.h"
#include "bull_math.h"


#include "default_vert.h"
#include "billboard_geo.h"
#include "billboard_vert.h"

#include "flat_texture_frag.h"
#include "steam_column_mesh.h"
#include "steam_frag.h"

#include "standard_material_frag.h"

#include "mist_frag.h"

#include "cooling_tower_mesh.h"
#include "tower_pipes_mesh.h"
#include "tower_discharges_mesh.h"

#define PLUME_PLANT_SCALE 50

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
  Transform const *const hierarchy_transform,
  EntityId id,
  ECS const *const ecs
);

static EntityId create_discharge_mist_spawner(
  EntityId cooling_tower,
  Vec3 position,
  ECS *const ecs
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

  steam_shader.frag_src = STEAM_FRAG_SRC;
  steam_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&steam_shader);
  gpu->copy_dynamic_mesh_to_gpu(&STEAM_COLUMN_MESH);

  solid_material_shader.frag_src = STANDARD_MATERIAL_FRAG_SRC;
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

EntityId create_plume_plant(
  Vec3 position,
  ECS *const ecs
) {

  EntityId cooling_tower = ecs__create_entity(ecs);

  ecs__add_transform(
    cooling_tower,
    (Transform){
      .position = position,
      .scale = PLUME_PLANT_SCALE
    },
    ecs
  );
  ecs__add_receives_light(cooling_tower, ecs);
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
      .scale = 1,
      .rotation = quaternion__create(WORLDSPACE.up, 0)
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
      .scale = 1
    },
    ecs
  );
  ecs__add_uv_scroll(
    discharges,
    (ScrollUV){
      .total = (Vec2){0},
      .speed = (Vec2){ 0, -0.05f }
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

  Vec3 initial_spawner_position = (Vec3){ 0, 0, 2.35f * PLUME_PLANT_SCALE };
  
  Vec3 spawner_offset;
  for (int i = 0; i < 4; i++) {

    spawner_offset = space__ccw_angle_rotate(
      WORLDSPACE.up,
      M_PI * 0.5f * i,
      initial_spawner_position
    );

    create_discharge_mist_spawner(
      cooling_tower,
      spawner_offset,
      ecs
    );

    // EntityId glow = ecs__create_entity(ecs);
    // ecs__add_transform(
    //   glow,
    //   (Transform){
    //     .position = spawner_offset
    //   },
    //   ecs
    // );
    // ecs__add_point_light_source(
    //   glow,
    //   (PointLight){
    //     .color = COLOR_RED,
    //     .strength = 300
    //   },
    //   ecs
    // );
  }

  EntityId plume = ecs__create_entity(ecs);

  ecs__add_parent_relationship(cooling_tower, plume, ecs);
  ecs__add_receives_light(plume, ecs);
  ecs__add_transform(
    plume,
    (Transform){
      .scale = 1,
      .position = (Vec3){0}
    },
    ecs
  );
  ecs__add_uv_scroll(
    plume,
    (ScrollUV){
      .speed = (Vec2){ -0.01f, 0 },
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

static EntityId create_discharge_mist_spawner(
  EntityId cooling_tower,
  Vec3 position,
  ECS *const ecs
) {

  EntityId spawner = ecs__create_entity(ecs);

  ecs__add_parent_relationship(cooling_tower, spawner, ecs);
  ecs__add_transform(
    spawner,
    (Transform){
      .position = position
    },
    ecs
  );
  ecs__add_repeat(
    spawner,
    (Repeat){
      .age = 0.5f,
      .interval = 0.5f,
      .on_interval = spawn_discharge_mist
    },
    ecs
  );

  return spawner;
}

static inline Vec3 generate_mist_position(
  Vec3 initial_position
) {
  static uint_fast8_t i;
  i = (i + 1) % 14;
  return vec3_plus_vec3(
    initial_position,
    space__ccw_angle_rotate(
      WORLDSPACE.up,
      (M_PI * 2.0f) * ((float)i / 14.0f),
      (Vec3){ 0, -5, -26 }
    )
  );
}
static void spawn_discharge_mist(
  EntityId spawner,
  Seconds remainder,
  ECS *const ecs
) {

  EntityId mist = ecs__create_entity(ecs);

  ecs__add_alpha_effect(mist, ecs);
  ecs__add_transform(
    mist,
    (Transform){
      .position = generate_mist_position(
        ecs->entities[spawner].transform.position
      ),
      .scale = 15
    },
    ecs
  );
  ecs__add_velocity(
    mist,
    (Vec3){ -1, 2.5f, -2 },
    ecs
  );
  ecs__add_receives_light(mist, ecs);
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
      .limit = 6,
      .on_timeout = destroy_mist
    },
    ecs
  );
}

static void draw_plume(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  Transform const *const hierarchy_transform,
  EntityId id,
  ECS const *const ecs
) {

  gpu->set_shader_float(
    ecs->entities[id].draw.shader,
    "min_altitude",
    300
  );
  gpu->set_shader_float(
    ecs->entities[id].draw.shader,
    "max_altitude",
    500
  );

  ecs__draw_mesh(
    time,
    camera,
    gpu,
    hierarchy_transform,
    id,
    ecs
  );
}

static void destroy_mist(
  EntityId mist,
  Seconds remainder,
  ECS *const ecs
) {
  ecs__mark_for_destruction(mist, ecs);
}