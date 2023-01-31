#include "tail.h"

#include "ecs.h"

#include "billboard_geo.h"
#include "billboard_vert.h"
#include "lowpoly_mushroom_cloud_mesh.h"
#include "fireball_texture.h"
#include "blink_texture.h"
#include "explosion_blink_frag.h"

static struct Shader rpg_explosion_blink_shader;
static struct Shader rpg_explosion_fireball_shader;

void explosions__copy_assets_to_gpu(
  struct GPU const *const gpu
) {

  rpg_explosion_blink_shader.frag_src = EXPLOSION_BLINK_FRAG_SRC;
  rpg_explosion_blink_shader.geo_src= BILLBOARD_GEO_SRC;
  rpg_explosion_blink_shader.vert_src = BILLBOARD_VERT_SRC;
  gpu->copy_shader_to_gpu(&rpg_explosion_blink_shader);

  gpu->copy_texture_to_gpu(&BLINK_TEXTURE);
}

void create_rpg_explosion(
  struct Vec3 position,
  void (*mark_entity_for_destruction)(
    EntityId id,
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
  ecs__add_draw_billboard(
    blink,
    (struct Draw){
      .texture = &BLINK_TEXTURE,
      .shader = &rpg_explosion_blink_shader
    },
    ecs
  );
  ecs__add_timeout(
    blink,
    (struct Timeout){
      .seconds_since_activation = 0,
      .limit_in_seconds = 0.15f,
      .on_timeout = mark_entity_for_destruction
    },
    ecs
  );
}