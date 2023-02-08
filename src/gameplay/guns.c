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
  struct Transform const *const source_transform,
  struct Vec3 target,
  Seconds duration,
  void (*on_reach_final_destination)(
    EntityId id,
    Seconds remainder,
    struct ECS *const ecs
  ),
  struct ECS *const ecs
) {

  EntityId cannonfire = ecs__create_entity(ecs);

  struct Quaternion face_quad_up = quaternion__create(
    WORLDSPACE.right,
    M_PI * 0.5f
  );

  ecs__add_transform(
    cannonfire,
    (struct Transform){
      .position = source_transform->position,
      .scale = 0.3f,
      .rotation = quaternion__multiply(
        source_transform->rotation,
        face_quad_up
      )
    },
    ecs
  );
  ecs__add_vec3lerp(
    cannonfire,
    (struct Vec3Lerp){
      .start = source_transform->position,
      .end = target,
      .age = 0,
      .duration = duration,
      .lerp = vec3__linear_lerp,
      .on_finish = on_reach_final_destination
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