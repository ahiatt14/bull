#include "tail.h"

#include "guns.h"

#include "ecs.h"

#include "constants.h"
#include "bull_math.h"

#include "bullets_texture.h"

void guns__copy_assets_to_gpu(
  struct GPU const *const gpu
) {
  gpu->copy_texture_to_gpu(&BULLETS_TEXTURE);
}

EntityId create_lvl0_cannonfire(
  struct Vec3 position,
  struct Vec3 direction,
  // Seconds remainder,
  void (*on_timeout)(
    EntityId id,
    Seconds remainder,
    struct ECS *const ecs
  ),
  struct ECS *const ecs
) {

  static const float speed = 30.0f;

  EntityId cannonfire = ecs__create_entity(ecs);

  struct Quaternion point_to_target = quaternion__create(
    WORLDSPACE.up,
    rads_ccw_from_forward_around_up(vec3__negate(direction))
  );

  struct Quaternion face_quad_up = quaternion__create(
    WORLDSPACE.right,
    M_PI * 0.5f
  );

  ecs__add_transform(
    cannonfire,
    (struct Transform){
      .position = position,
      .scale = 1.3f,
      .rotation = quaternion__multiply(
        point_to_target,
        face_quad_up
      )
    },
    ecs
  );
  ecs__add_timeout(
    cannonfire,
    (struct Timeout){
      .age = 0, // TODO: remainder??
      .limit = 0.2f,
      .on_timeout = on_timeout
    },
    ecs
  );
  ecs__add_velocity(
    cannonfire,
    scalar_x_vec3(speed, direction),
    ecs
  );
  ecs__add_draw(
    cannonfire,
    (struct Draw){
      .mesh = &QUAD,
      .texture = &BULLETS_TEXTURE,
      .shader = &FLAT_TEXTURE_SHADER
    },
    ecs
  );

  return cannonfire;
}