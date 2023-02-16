#include "tail.h"

#include "guns.h"

#include "ecs.h"

#include "constants.h"
#include "bull_math.h"

#include "bullets_texture.h"

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
  gpu->copy_texture_to_gpu(&BULLETS_TEXTURE);
}

EntityId create_lvl0_cannonfire(
  struct Vec3 position,
  struct Vec3 direction,
  Seconds remainder,
  struct ECS *const ecs
) {

  static const double DURATION = 0.5f;
  static const float SPEED = 30.0f;

  EntityId bullet = ecs__create_entity(ecs);

  struct Quaternion point_to_target = quaternion__create(
    WORLDSPACE.up,
    rads_ccw_from_forward_around_up(vec3__negate(direction))
  );

  struct Quaternion face_quad_up = quaternion__create(
    WORLDSPACE.right,
    M_PI * 0.5f
  );

  struct Vec3 velocity = scalar_x_vec3(SPEED, direction);

  ecs__add_transform(
    bullet,
    (struct Transform){
      .position = vec3_plus_vec3(
        position,
        scalar_x_vec3(remainder, velocity)
      ),
      .scale = 2.5f,
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
  ecs__add_projectile_radius_collider(
    bullet,
    (struct RadiusCollider){
      .radius = 0.1f,
      .on_collide = NULL
    },
    ecs
  );
  ecs__add_draw(
    bullet,
    (struct Draw){
      .mesh = &QUAD,
      .texture = &BULLETS_TEXTURE,
      .shader = &FLAT_TEXTURE_SHADER,
      .draw = ecs__draw_mesh
    },
    ecs
  );

  return bullet;
}