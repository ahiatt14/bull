#include <stdio.h>
#include "tail.h"

#include "guns.h"

#include "ecs.h"

#include "assets.h"
#include "constants.h"
#include "bull_math.h"

#define Z_OFFSET 0.15f

static void destroy_bullet(
  EntityId bullet,
  Seconds remainder,
  struct ECS *const ecs
) {
  ecs__mark_for_destruction(bullet, ecs);
}

void guns__copy_assets_to_gpu(
  struct GPU const *const gpu
) {

}

EntityId create_lvl0_cannonfire(
  struct Vec3 position,
  struct Vec3 direction,
  Seconds remainder,
  struct ECS *const ecs
) {

  static uint_fast8_t offset_z;

  static const double DURATION = 0.5f;
  static const float SPEED = 50.0f;

  EntityId bullet = ecs__create_entity(ecs);

  struct Quaternion point_to_target = quaternion__create(
    WORLDSPACE.up,
    rads_ccw_from_forward_around_up(direction)
  );

  struct Quaternion face_quad_up = quaternion__create(
    WORLDSPACE.right,
    -M_PI * 0.5f
  );

  struct Vec3 velocity = scalar_x_vec3(SPEED, direction);

  offset_z = offset_z ? 0 : 1;
  struct Vec3 z_offset = { 0, offset_z ? Z_OFFSET : 0, 0 };
  struct Vec3 offset_position = vec3_plus_vec3(
    z_offset,
    vec3_plus_vec3(position, scalar_x_vec3(remainder, velocity))
  );

  ecs__add_transform(
    bullet,
    (struct Transform){
      .position = offset_position,
      .scale = 1.2f,
      .rotation = quaternion__multiply(
        point_to_target,
        face_quad_up
      )
    },
    ecs
  );
  ecs__add_velocity(
    bullet,
    velocity,
    ecs
  );
  ecs__add_timeout(
    bullet,
    (struct Timeout){
      .age = remainder,
      .limit = DURATION,
      .on_timeout = destroy_bullet
    },
    ecs
  );
  ecs__add_radius_collider(bullet, 0.1f, ecs);
  ecs__add_damager(bullet, ecs);
  // ecs__add_alpha_effect(bullet, ecs);
  ecs__add_draw(
    bullet,
    (struct Draw){
      .mesh = &QUAD,
      .textures = BULLETS_TEXTURE,
      .shader = &FLAT_TEXTURE_SHADER,
      .draw = ecs__draw_mesh
    },
    ecs
  );

  return bullet;
}