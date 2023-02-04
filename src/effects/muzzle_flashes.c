#include "tail.h"

#include "ecs.h"

#include "constants.h"

#include "billboard_geo.h"
#include "billboard_vert.h"

#include "muzzle_flash_texture.h"
#include "explosion_blink_frag.h"

static struct Shader muzzle_flash_blink_shader;

void muzzle_flashes__copy_assets_to_gpu(
  struct GPU const *const gpu
) {

  muzzle_flash_blink_shader.frag_src = EXPLOSION_BLINK_FRAG_SRC;
  muzzle_flash_blink_shader.geo_src= BILLBOARD_GEO_SRC;
  muzzle_flash_blink_shader.vert_src = BILLBOARD_VERT_SRC;
  gpu->copy_shader_to_gpu(&muzzle_flash_blink_shader);

  gpu->copy_texture_to_gpu(&MUZZLE_FLASH_TEXTURE);
}

void create_muzzle_flash(
  struct Vec3 position,
  void (*mark_entity_for_destruction)(
    EntityId id,
    struct ECS *const ecs
  ),
  struct ECS *const ecs
) {

  EntityId flash = ecs__create_entity(ecs);

  ecs__add_transform(
    flash,
    (struct Transform){
      .position = position,
      .scale = 1
    },
    ecs
  );
  ecs__add_timeout(
    flash,
    (struct Timeout){
      .seconds_since_activation = 0,
      .limit_in_seconds = 0.1f,
      .on_timeout = mark_entity_for_destruction
    },
    ecs
  );
  ecs__add_draw_billboard(
    flash,
    (struct Draw){
      .texture = &MUZZLE_FLASH_TEXTURE,
      .shader = &muzzle_flash_blink_shader
    },
    ecs
  );
}