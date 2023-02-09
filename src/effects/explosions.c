#include "tail.h"

#include "ecs.h"

#include "constants.h"

#include "billboard_geo.h"
#include "billboard_vert.h"
#include "default_vert.h"
#include "lowpoly_mushroom_cloud_mesh.h"
#include "fireball_texture.h"
#include "blink_texture.h"
#include "explosion_blink_frag.h"
#include "explosion_frag.h"
#include "mushroom_cloud_frag.h"

static struct Shader rpg_explosion_blink_shader;
static struct Shader rpg_explosion_mushroom_cloud_shader;

void explosions__copy_assets_to_gpu(
  struct GPU const *const gpu
) {

  gpu->copy_static_mesh_to_gpu(&LOWPOLY_MUSHROOM_CLOUD_MESH);

  rpg_explosion_blink_shader.frag_src = EXPLOSION_BLINK_FRAG_SRC;
  rpg_explosion_blink_shader.geo_src= BILLBOARD_GEO_SRC;
  rpg_explosion_blink_shader.vert_src = BILLBOARD_VERT_SRC;
  gpu->copy_shader_to_gpu(&rpg_explosion_blink_shader);

  rpg_explosion_mushroom_cloud_shader.frag_src = EXPLOSION_FRAG_SRC;
  rpg_explosion_mushroom_cloud_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&rpg_explosion_mushroom_cloud_shader);

  gpu->copy_texture_to_gpu(&BLINK_TEXTURE);
  gpu->copy_texture_to_gpu(&FIREBALL_TEXTURE);
}

void create_rpg_explosion(
  struct Vec3 position,
  void (*mark_entity_for_destruction)(
    EntityId id,
    Seconds remainder,
    struct ECS *const ecs
  ),
  struct ECS *const ecs
) {

  EntityId blink = ecs__create_entity(ecs);

  ecs__add_transform(
    blink,
    (struct Transform){
      .position = position,
      .scale = 1
    },
    ecs
  );
  ecs__add_timeout(
    blink,
    (struct Timeout){
      .age = 0,
      .limit = 0.15f,
      .on_timeout = mark_entity_for_destruction
    },
    ecs
  );
  ecs__add_draw_billboard(
    blink,
    (struct Draw){
      .texture = &BLINK_TEXTURE,
      .shader = &rpg_explosion_blink_shader
    },
    ecs
  );

  EntityId mushroom_cloud = ecs__create_entity(ecs);

  ecs__add_transform(
    mushroom_cloud,
    (struct Transform){
      .position = position,
      .rotation = quaternion__create(
        WORLDSPACE.up,
        0
      ),
      .scale = 0.5f
    },
    ecs
  );
  ecs__add_timeout(
    mushroom_cloud,
    (struct Timeout){
      .age = 0,
      .limit = 1,
      .on_timeout = mark_entity_for_destruction
    },
    ecs
  );
  ecs__add_velocity(
    mushroom_cloud,
    (struct Vec3){ 0, 1, 0 },
    ecs
  );
  ecs__add_uv_scroll(
    mushroom_cloud,
    (struct Vec2){ 0, 1 },
    ecs
  );
  ecs__add_draw(
    mushroom_cloud,
    (struct Draw){
      .texture = &FIREBALL_TEXTURE,
      .shader = &rpg_explosion_mushroom_cloud_shader,
      .mesh = &LOWPOLY_MUSHROOM_CLOUD_MESH
    },
    ecs
  );
}