#include <stdio.h>

#include "tail.h"

#include "rpg.h"

#include "ecs.h"

#include "assets.h"
#include "bull_math.h"
#include "constants.h"

EntityId deploy_rpg(
  Vec3 position,
  void (*on_rpg_deployed)(
    EntityId id,
    Seconds remainder,
    ECS *const ecs
  ),
  ECS *const ecs
) {

  EntityId rocket = ecs__create_entity(ecs);
  static uint_fast8_t deploy_right = 0;

  Vec3 forward =
    vec3__normalize(vec3_minus_vec3(ORIGIN, position));

  Vec3 right =
    vec3__normalize(vec3__cross(forward, WORLDSPACE.up));

  Vec3 propel_position =
    deploy_right ?
    vec3_plus_vec3(position, right) :
    vec3_plus_vec3(position, vec3__negate(right));
  deploy_right = deploy_right ? 0 : 1;

  ecs__add_transform(
    rocket,
    (Transform){
      .position = position,
      .rotation = quaternion__create(
        WORLDSPACE.up,
        rads_ccw_from_forward_around_up(forward)
      ),
      .scale = 1
    },
    ecs
  );
  ecs__add_draw(
    rocket,
    (Draw){
      .mesh = &ROCKET_MESH,
      .textures = 0,
      .shader = &SOLID_COLOR_SHADER,
      .draw = ecs__draw_mesh
    },
    ecs
  );
  ecs__add_vec3lerp(
    rocket,
    (Vec3Lerp){
      .start = position,
      .end = propel_position,
      .age = 0,
      .duration = 0.15f,
      .lerp = vec3__linear_lerp,
      .on_finish = on_rpg_deployed
    },
    ecs
  );

  return rocket;
}

void propel_rpg(
  EntityId rocket,
  Seconds remainder,
  void (*on_rpg_timer_up)(
    EntityId id,
    Seconds remainder,
    ECS *const ecs
  ),
  ECS *const ecs
) {

  Vec3 position =
    ecs->entities[rocket].transform.position;
  Quaternion rotation =
    ecs->entities[rocket].transform.rotation;

  Vec3 forward =
    vec3__normalize(space__ccw_quat_rotate(
      rotation,
      WORLDSPACE.forward
    ));

  Vec3 end = vec3_plus_vec3(
    position,
    scalar_x_vec3(10.0f, forward)
  );

  ecs->entities[rocket].vec3lerp = (Vec3Lerp){
    .start = position, // TODO: give headstart with lerp remainder
    .end = end,
    .age = 0, // TODO: give headstart with lerp remainder?
    .duration = 0.5f,
    .lerp = vec3__square_lerp,
    .on_finish = on_rpg_timer_up
  };
}

void create_rpg_thruster_blink(
  EntityId rocket,
  ECS *const ecs
) {
  
}