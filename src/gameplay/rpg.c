#include <stdio.h>

#include "tail.h"

#include "rpg.h"

#include "ecs.h"

#include "bull_math.h"
#include "constants.h"

#include "rocket_mesh.h"

EntityId deploy_rpg(
  struct Vec3 position,
  void (*on_deployed)(
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
      .on_finish = on_deployed
    },
    ecs
  );

  return rocket;
}

// ecs__add_timeout(
//   rocket,
//   (struct Timeout){
//     .limit_in_seconds = 1,
//     .seconds_since_activation = 0,
//     .on_timeout = on_hit_limit
//   },
//   ecs
// );