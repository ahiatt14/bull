#include "cloud_cover.h"

#include "tail.h"

#include "ecs.h"

#include "assets.h"
#include "constants.h"

#include "cloud_cover_frag.h"
#include "standard_material_frag.h"
#include "default_vert.h"

void draw_cloud_cover(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  Transform const *const hierarchy_transform,
  EntityId id,
  ECS const *const ecs
);

static Shader shader;

void cloud_cover__copy_assets_to_gpu(
  GPU const *const gpu
) {
  shader.frag_src = CLOUD_COVER_FRAG_SRC;
  shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&shader);
}

void create_cloud_cover(
  ECS *const ecs
) {
  EntityId cloud_cover = ecs__create_entity(ecs);
  ecs__add_transform(
    cloud_cover,
    (Transform){
      .position = (Vec3){ 0, 700, 0 },
      .scale = 3500,
      .rotation = quaternion__create(WORLDSPACE.up, 0)
    },
    ecs
  );
  ecs__add_receives_light(cloud_cover, ecs);
  // ecs__add_alpha_effect(cloud_cover, ecs);
  ecs__add_uv_scroll(
    cloud_cover,
    (ScrollUV){
      .total = (Vec2){0},
      .speed = (Vec2){ -0.01f, 0 }
    },
    ecs
  );
  ecs__add_draw(
    cloud_cover,
    (Draw){
      .mesh = &CLOUD_COVER_MESH,
      .draw = draw_cloud_cover,
      .textures = CLOUDS_TEXTURE,
      .shader = &shader
    },
    ecs
  );
}

void draw_cloud_cover(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  Transform const *const hierarchy_transform,
  EntityId id,
  ECS const *const ecs
) {

  gpu->set_shader_float(
    ecs->entities[id].draw.shader,
    "min_distance",
    1000
  );
  gpu->set_shader_float(
    ecs->entities[id].draw.shader,
    "max_distance",
    3000
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