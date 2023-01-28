#include "tail.h"

#include "rpg.h"

#include "ecs.h"

#include "bull_math.h"
#include "constants.h"

#include "rocket_mesh.h"

EntityId create_rpg(
  struct vec3 position,
  struct vec3 velocity,
  void (*mark_entity_for_destruction)(EntityId),
  struct ECS *const ecs
) {

  EntityId rocket = ecs__create_entity(ecs);

  struct vec3 player_to_origin =
    vec3__normalize(vec3_minus_vec3(
      ORIGIN,
      position
    ));

  ecs__add_transform(
    rocket,
    (struct transform){
      .position = position,
      ._rotation = quaternion__create(
        WORLDSPACE.up,
        rads_ccw_from_forward_around_up(player_to_origin)
      ),
      .scale = 1
    },
    ecs
  );
  ecs__add_timeout(
    rocket,
    (struct Timeout){
      .limit_in_seconds = 1,
      .seconds_since_activation = 0,
      .on_timeout = mark_entity_for_destruction
    },
    ecs
  );
  ecs__add_draw_mesh(
    rocket,
    (struct Draw){
      .mesh = &rocket_mesh,
      .texture = NULL,
      .shader = &SOLID_COLOR_SHADER
    },
    ecs
  );
  ecs__add_velocity(
    rocket,
    scalar_x_vec3(10.0f, player_to_origin),
    ecs
  );

  return rocket;
}