#include <stdlib.h>
#include <math.h>

#include "tail.h"
#include "ecs.h"

#include "debris.h"

#include "assets.h"
#include "constants.h"
#include "bull_math.h"

static void destroy_debris(
  EntityId debris,
  Seconds remainder,
  ECS *const ecs
) {
  ecs__mark_for_destruction(debris, ecs);
}

static Vec3 random_direction() {
  return vec3__normalize((Vec3){
    0.5f - (rand() / (float)RAND_MAX),
    (rand() / (float)RAND_MAX),
    0.5f - (rand() / (float)RAND_MAX)
  });
}

void create_debris(
  Vec3 position,
  Vec3 source_velocity,
  uint_fast8_t count,
  ECS *const ecs
) {

  EntityId debris;
  
  Vec3 directions_to_average[2] = {
    scalar_x_vec3(0.1f, source_velocity),
    (Vec3){0}
  };

  // Quaternion point_to_target;
  Vec3 averaged_velocity;

  for (uint_fast8_t i = 0; i < count; i++) {

    debris = ecs__create_entity(ecs);

    // TODO: pre-calculate and store a bunch of directions
    // to consume instead of randoming every one every time
    directions_to_average[1] =
      scalar_x_vec3(5, random_direction());
    averaged_velocity = vec3__mean(directions_to_average, 2);

    ecs__add_transform(
      debris,
      (Transform){
        .position = position,
        .scale = 0.15
      },
      ecs
    );
    ecs__add_rotation_lerp(
      debris,
      (RotationLerp){
        .age = 0,
        .duration = 2,
        .start = quaternion__create(WORLDSPACE.up, 0),
        .target = quaternion__create(
          vec3__cross(
            vec3__normalize(averaged_velocity),
            WORLDSPACE.up
          ),
          M_PI * 4.0
        ),
        .on_finish = destroy_debris
      },
      ecs
    );
    ecs__add_velocity(
      debris,
      averaged_velocity,
      ecs
    );
    ecs__add_gravity(debris, ecs);
    ecs__add_draw(
      debris,
      (Draw){
        .textures = BLASTED_STONE_TEXTURE,
        .shader = &FLAT_TEXTURE_SHADER,
        .draw = ecs__draw_mesh,
        .mesh = &QUAD
      },
      ecs
    );
  }
}