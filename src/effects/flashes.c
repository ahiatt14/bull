#include "tail.h"

#include "ecs.h"

#include "flashes.h"

#include "constants.h"
#include "assets.h"

#include "billboard_geo.h"
#include "billboard_vert.h"
#include "flash_frag.h"

static Shader blue_pulse_shader;

static void destroy_flash(
  EntityId id,
  Seconds remainder,
  ECS *const ecs
) {
  ecs__mark_for_destruction(id, ecs);
}

void flashes__copy_assets_to_gpu(
  GPU const *const gpu
) {
  blue_pulse_shader.frag_src = FLASH_FRAG_SRC;
  blue_pulse_shader.vert_src = BILLBOARD_VERT_SRC;
  blue_pulse_shader.geo_src = BILLBOARD_GEO_SRC;
  gpu->copy_shader_to_gpu(&blue_pulse_shader);
}

EntityId create_blue_pulse(
  Vec3 position,
  ECS *const ecs
) {

  EntityId pulse = ecs__create_entity(ecs);

  ecs__add_transform(
    pulse,
    (Transform){
      .position = vec3_plus_vec3(
        position,
        (Vec3){ 0, 1, 0 }
      ),
      .scale = 3
    },
    ecs
  );
  ecs__add_alpha_effect(pulse, ecs);
  ecs__add_draw(
    pulse,
    (Draw){
      .draw = ecs__draw_billboard,
      .shader = &blue_pulse_shader,
      .textures = BLUE_PULSE_TEXTURE
    },
    ecs
  );
  ecs__add_timeout(
    pulse,
    (Timeout){
      .age = 0,
      .limit = 0.2f,
      .on_timeout = destroy_flash
    },
    ecs
  );

  return pulse;
}