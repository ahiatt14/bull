#include <stdio.h>

#include "tail.h"

#include "rpg.h"

#include "ecs.h"

#include "bull_math.h"
#include "constants.h"

#include "rocket_mesh.h"

EntityId deploy_rpg(
  struct Vec3 position,
  void (*on_rpg_deployed)(
    EntityId id,
    double remainder_in_seconds,
    struct ECS *const ecs
  ),
  struct ECS *const ecs
) {

  EntityId rocket = ecs__create_entity(ecs);
  static uint_fast8_t deploy_right = 0;

  struct Vec3 forward =
    vec3__normalize(vec3_minus_vec3(ORIGIN, position));

  struct Vec3 right =
    vec3__normalize(vec3__cross(forward, WORLDSPACE.up));

  struct Vec3 propel_position =
    deploy_right ?
    vec3_plus_vec3(position, right) :
    vec3_plus_vec3(position, vec3__negate(right));
  deploy_right = deploy_right ? 0 : 1;

  ecs__add_transform(
    rocket,
    (struct Transform){
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
    (struct Draw){
      .mesh = &ROCKET_MESH,
      .texture = NULL,
      .shader = &SOLID_COLOR_SHADER
    },
    ecs
  );
  ecs__add_vec3lerp(
    rocket,
    (struct Vec3Lerp){
      .start = position,
      .end = propel_position,
      .seconds_since_activation = 0,
      .duration_in_seconds = 0.15f,
      .lerp = vec3__linear_lerp,
      .on_finish = on_rpg_deployed
    },
    ecs
  );

  return rocket;
}

void propel_rpg(
  EntityId rocket,
  double remainder_in_seconds,
  void (*on_rpg_timer_up)(
    EntityId id,
    double remainder_in_seconds,
    struct ECS *const ecs
  ),
  struct ECS *const ecs
) {

  struct Vec3 position =
    ecs->entities[rocket].transform.position;
  struct Quaternion rotation =
    ecs->entities[rocket].transform.rotation;

  struct Vec3 forward =
    vec3__normalize(space__ccw_quat_rotate(
      rotation,
      WORLDSPACE.forward
    ));

  struct Vec3 end = vec3_plus_vec3(
    position,
    scalar_x_vec3(10.0f, forward)
  );

  ecs->entities[rocket].vec3lerp = (struct Vec3Lerp){
    .start = position, // TODO: give headstart with lerp remainder
    .end = end,
    .seconds_since_activation = 0, // TODO: give headstart with lerp remainder?
    .duration_in_seconds = 0.5f,
    .lerp = vec3__square_lerp,
    .on_finish = on_rpg_timer_up
  };
}

void create_rpg_thruster_blink(
  EntityId rocket,
  struct ECS *const ecs
) {
  
}