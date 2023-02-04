#include <math.h>

#include "tail.h"

#include "ecs.h"

#include "constants.h"

#include "default_vert.h"
#include "billboard_geo.h"
#include "billboard_vert.h"

#include "muzzle_flash_texture.h"
#include "explosion_blink_frag.h"

static struct Shader muzzle_flash_blink_shader;

void muzzle_flashes__copy_assets_to_gpu(
  struct GPU const *const gpu
) {

  muzzle_flash_blink_shader.frag_src = EXPLOSION_BLINK_FRAG_SRC;
  muzzle_flash_blink_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&muzzle_flash_blink_shader);

  gpu->copy_texture_to_gpu(&MUZZLE_FLASH_TEXTURE);
}

void create_lvl0_muzzle_flash(
  struct Transform const *const source_transform,
  void (*mark_entity_for_destruction)(
    EntityId id,
    struct ECS *const ecs
  ),
  struct ECS *const ecs
) {

  struct Vec3 source_forward = space__ccw_quat_rotate(
    source_transform->rotation,
    WORLDSPACE.forward
  );

  struct Quaternion right_rotation = quaternion__create(
    WORLDSPACE.right,
    -M_PI * 0.5f
  );

  EntityId flash = ecs__create_entity(ecs);

  ecs__add_transform(
    flash,
    (struct Transform){
      .position = vec3_plus_vec3(
        source_transform->position,
        scalar_x_vec3(0.5f, source_forward)
      ),
      .rotation = quaternion__multiply(
        source_transform->rotation,
        right_rotation
      ),
      .scale = 1.5f
    },
    ecs
  );
  ecs__add_timeout(
    flash,
    (struct Timeout){
      .seconds_since_activation = 0,
      .limit_in_seconds = 0.15f,
      .on_timeout = mark_entity_for_destruction
    },
    ecs
  );
  ecs__add_draw(
    flash,
    (struct Draw){
      .texture = &MUZZLE_FLASH_TEXTURE,
      .mesh = &QUAD,
      .shader = &muzzle_flash_blink_shader
    },
    ecs
  );
}