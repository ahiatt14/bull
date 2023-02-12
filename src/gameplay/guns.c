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
  Seconds remainder,
  void (*mark_entity_for_destruction)(
    EntityId id,
    Seconds remainder,
    struct ECS *const ecs
  ),
  struct ECS *const ecs
) {

  static const double DURATION = 0.5f;

  EntityId cannonfire = ecs__create_entity(ecs);

  struct Quaternion point_to_target = quaternion__create(
    WORLDSPACE.up,
    rads_ccw_from_forward_around_up(vec3__negate(direction))
  );

  struct Quaternion face_quad_up = quaternion__create(
    WORLDSPACE.right,
    M_PI * 0.5f
  );

  struct Vec3 destination = vec3_plus_vec3(
    position,
    scalar_x_vec3(20, direction)
  );

  ecs__add_transform(
    cannonfire,
    (struct Transform){
      .scale = 1.6f,
      .rotation = quaternion__multiply(
        point_to_target,
        face_quad_up
      )
    },
    ecs
  );
  ecs__add_vec3lerp(
    cannonfire,
    (struct Vec3Lerp){
      .start = vec3__linear_lerp(
        position,
        destination,
        remainder / DURATION
      ),
      .end = destination,
      .age = remainder,
      .duration = DURATION,
      .lerp = vec3__linear_lerp,
      .on_finish = mark_entity_for_destruction
    },
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