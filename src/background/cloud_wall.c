#include <math.h>

#include "cloud_wall.h"

#include "tail.h"

#include "ecs.h"

#include "assets.h"
#include "constants.h"

#include "steam_frag.h"
#include "default_vert.h"

void draw_cloud_wall(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  Transform const *const hierarchy_transform,
  EntityId id,
  ECS const *const ecs
);

static Shader shader;

void cloud_wall__copy_assets_to_gpu(
  GPU const *const gpu
) {
  shader.frag_src = STEAM_FRAG_SRC;
  shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&shader);
}

void create_cloud_wall(
  ECS *const ecs
) {
  EntityId cloud_wall = ecs__create_entity(ecs);
  ecs__add_transform(
    cloud_wall,
    (Transform){
      .position = (Vec3){ 0, 50, 0 },
      .scale = 700,
      .rotation = quaternion__create(WORLDSPACE.up, M_PI)
    },
    ecs
  );
  ecs__add_uv_scroll(
    cloud_wall,
    (ScrollUV){
      .total = (Vec2){0},
      .speed = (Vec2){ -0.005f, 0 }
    },
    ecs
  );
  ecs__add_receives_light(cloud_wall, ecs);
  // ecs__add_alpha_effect(cloud_wall, ecs);
  ecs__add_draw(
    cloud_wall,
    (Draw){
      .mesh = &CLOUD_WALL_MESH,
      .draw = draw_cloud_wall,
      .textures = STEAM_TEXTURE,
      .shader = &shader
    },
    ecs
  );
}

void draw_cloud_wall(
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