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

#include "cooling_tower_mesh.h"

static Shader steam_shader;
static Shader solid_material_shader;

void plume_plant__copy_assets_to_gpu(
  GPU const *const gpu
) {

  steam_shader.frag_src = FLAT_TEXTURE_FRAG_SRC;
  steam_shader.geo_src = STEAM_GEO_SRC;
  steam_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&steam_shader);
  gpu->copy_dynamic_mesh_to_gpu(&STEAM_COLUMN_MESH);

  gpu->copy_static_mesh_to_gpu(&COOLING_TOWER_MESH);

  solid_material_shader.frag_src = MOUNTAIN_FRAG_SRC;
  solid_material_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&solid_material_shader);
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
      .scale = 25
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
      .scale = 250,
      .position = vec3_plus_vec3(
        position,
        (Vec3){ 0, 10, 0 }
      )
    },
    ecs
  );
  ecs__add_uv_scroll(
    steam,
    (ScrollUV){
      .speed = (Vec2){ 0.05f, 0.05f },
      .total = (Vec2){0}
    },
    ecs
  );
  ecs__add_draw(
    steam,
    (Draw){
      .textures = STEAM_TEXTURE,
      .draw = ecs__draw_mesh,
      .shader = &steam_shader,
      .mesh = &STEAM_COLUMN_MESH
    },
    ecs
  );

  return cooling_tower;
}