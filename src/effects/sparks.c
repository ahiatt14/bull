#include <stdlib.h>

#include "tail.h"
#include "ecs.h"

#include "sparks.h"

#include "assets.h"
#include "constants.h"
#include "bull_math.h"

static void destroy_spark(
  EntityId spark,
  Seconds remainder,
  struct ECS *const ecs
) {
  ecs__mark_for_destruction(spark, ecs);
}

static struct Vec3 random_direction() {
  return vec3__normalize((struct Vec3){
    0.5f - (rand() / (float)RAND_MAX),
    (rand() / (float)RAND_MAX),
    0.5f - (rand() / (float)RAND_MAX)
  });
}

void create_sparks(
  struct Vec3 position,
  struct Vec3 velocity,
  uint_fast8_t count,
  struct ECS *const ecs
) {

  EntityId spark;
  
  struct Vec3 directions_to_average[2] = {
    scalar_x_vec3(0.7f, velocity),
    (struct Vec3){0}
  };

  for (uint_fast8_t i = 0; i < count; i++) {

    spark = ecs__create_entity(ecs);

    // TODO: pre-calculate and store a bunch of directions
    // to consume
    directions_to_average[1] =
      scalar_x_vec3(30, random_direction());

    ecs__add_transform(
      spark,
      (struct Transform){
        .position = position,
        .scale = 0.1
      },
      ecs
    );
    ecs__add_velocity(
      spark,
      vec3__mean(directions_to_average, 2),
      ecs
    );
    ecs__add_draw(
      spark,
      (struct Draw){
        .texture = TEXTURES[SMALL_SPARK_TEXTURE],
        .shader = &DEFAULT_BILLBOARD_SHADER,
        .draw = ecs__draw_billboard
      },
      ecs
    );
    // ecs__add_alpha_effect(spark, ecs);
    ecs__add_timeout(
      spark,
      (struct Timeout){
        .age = 0,
        .limit = 0.15f,
        .on_timeout = destroy_spark
      },
      ecs
    );
  }
}