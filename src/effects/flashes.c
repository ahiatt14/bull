#include <math.h>
#include <stdio.h>

#include "tail.h"

#include "ecs.h"

#include "flashes.h"

#include "colors.h"
#include "constants.h"
#include "assets.h"
#include "bull_math.h"

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
        (Vec3){ 0, 0, 0 }
      ),
      .scale = 2
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

void create_autocannon_muzzle_flash(
  EntityId weapon,
  Seconds remainder,
  ECS *const ecs
) {

  EntityId flash = ecs__create_entity(ecs);

  ecs__add_parent_relationship(weapon, flash, ecs);
  ecs__add_transform(
    flash,
    (Transform){
      .position = (Vec3){ 0, 0, -0.8f },
      .scale = 2.4f,
      .rotation = quaternion__create(WORLDSPACE.up, 0)
    },
    ecs
  );
  ecs__add_timeout(
    flash,
    (Timeout){
      .age = remainder,
      .limit = 0.12f,
      .on_timeout = destroy_flash
    },
    ecs
  );
  ecs__add_uv_scroll(
    flash,
    (ScrollUV){
      .total = (Vec2){0},
      .speed = (Vec2){ 7.5f, 0 }
    },
    ecs
  );
  // ecs__add_point_light_source(
  //   flash,
  //   (PointLight){
  //     .color = COLOR_EVENING_SUNLIGHT,
  //     .strength = 100
  //   },
  //   ecs
  // );
  ecs__add_draw(
    flash,
    (Draw){
      .draw = ecs__draw_mesh,
      .mesh = &MUZZLE_FLASH_MESH,
      .textures = MUZZLE_FLASH_TEXTURE,
      .shader = &FLAT_TEXTURE_SHADER
    },
    ecs
  );
}