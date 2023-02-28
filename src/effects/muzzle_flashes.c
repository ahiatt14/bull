#include <math.h>

#include "tail.h"

#include "ecs.h"

#include "assets.h"
#include "constants.h"

#include "default_vert.h"
#include "billboard_geo.h"
#include "billboard_vert.h"

#include "explosion_blink_frag.h"

static Shader muzzle_flash_blink_shader;

static void destroy_flash(
  EntityId flash,
  Seconds remainder,
  ECS *const ecs
) {
  ecs__mark_for_destruction(flash, ecs);
}

void muzzle_flashes__copy_assets_to_gpu(
  GPU const *const gpu
) {

  muzzle_flash_blink_shader.frag_src = EXPLOSION_BLINK_FRAG_SRC;
  muzzle_flash_blink_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&muzzle_flash_blink_shader);
}

void create_lvl0_muzzle_flash(
  Transform const *const source_transform,
  ECS *const ecs
) {

  Vec3 source_forward = space__ccw_quat_rotate(
    source_transform->rotation,
    WORLDSPACE.forward
  );

  Quaternion right_rotation = quaternion__create(
    WORLDSPACE.right,
    -M_PI * 0.5f
  );

  EntityId flash = ecs__create_entity(ecs);

  ecs__add_transform(
    flash,
    (Transform){
      .position = vec3_plus_vec3(
        vec3_plus_vec3(
          source_transform->position,
          scalar_x_vec3(0.5, source_forward)
        ),
        (Vec3){0, 0, 0.1f}
      ),
      .rotation = quaternion__multiply(
        source_transform->rotation,
        right_rotation
      ),
      .scale = 1
    },
    ecs
  );
  ecs__add_timeout(
    flash,
    (Timeout){
      .age = 0,
      .limit = 1.0f / 60.0f,
      .on_timeout = destroy_flash
    },
    ecs
  );
  ecs__add_draw(
    flash,
    (Draw){
      .textures = 0,
      .mesh = &QUAD,
      .shader = &muzzle_flash_blink_shader,
      .draw = ecs__draw_mesh
    },
    ecs
  );
}