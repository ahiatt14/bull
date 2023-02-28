#include <stdlib.h>
#include <math.h>

#include "tail.h"
#include "ecs.h"

#include "sparks.h"

#include "assets.h"
#include "constants.h"
#include "bull_math.h"

static void destroy_spark(
  EntityId spark,
  Seconds remainder,
  ECS *const ecs
) {
  ecs__mark_for_destruction(spark, ecs);
}

static Vec3 random_direction() {
  return vec3__normalize((Vec3){
    0.5f - (rand() / (float)RAND_MAX),
    (rand() / (float)RAND_MAX),
    0.5f - (rand() / (float)RAND_MAX)
  });
}

void create_sparks(
  Vec3 position,
  Vec3 source_velocity,
  uint_fast8_t count,
  ECS *const ecs
) {

  EntityId spark;
  
  Vec3 directions_to_average[2] = {
    scalar_x_vec3(0.7f, source_velocity),
    (Vec3){0}
  };

  Quaternion point_to_target;
  Vec3 averaged_velocity;

  for (uint_fast8_t i = 0; i < count; i++) {

    spark = ecs__create_entity(ecs);

    // TODO: pre-calculate and store a bunch of directions
    // to consume instead of randoming every one every time
    directions_to_average[1] =
      scalar_x_vec3(30, random_direction());
    averaged_velocity = vec3__mean(directions_to_average, 2);

    point_to_target = quaternion__create(
      WORLDSPACE.forward,
      -rads_ccw_from_forward_around_up(averaged_velocity)
    );

    ecs__add_transform(
      spark,
      (Transform){
        .position = position,
        .rotation = point_to_target,
        .scale = 0.15
      },
      ecs
    );
    ecs__add_velocity(
      spark,
      averaged_velocity,
      ecs
    );
    ecs__add_alpha_effect(spark, ecs);
    ecs__add_draw(
      spark,
      (Draw){
        .textures = SMALL_SPARK_TEXTURE,
        .shader = &DEFAULT_BILLBOARD_SHADER,
        .draw = ecs__draw_billboard
      },
      ecs
    );
    ecs__add_timeout(
      spark,
      (Timeout){
        .age = 0,
        .limit = 0.15f,
        .on_timeout = destroy_spark
      },
      ecs
    );
  }
}