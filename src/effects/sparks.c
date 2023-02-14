#include <stdlib.h>
#include <stdio.h>

#include "tail.h"

#include "sparks.h"

#include "ecs.h"

#include "tiny_debris_texture.h"

static void destroy_spark(
  EntityId spark,
  Seconds remainder,
  struct ECS *const ecs
) {
  ecs__mark_for_destruction(spark, ecs);
}

struct Vec3 random_direction() {
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
  struct Transform transform = {
    .position = position,
    .scale = 0.05f
  };
  struct Vec3 directions_to_average[2] = {
    scalar_x_vec3(0.2f, velocity),
    (struct Vec3){0}
  };

  for (uint_fast8_t i = 0; i < count; i++) {

    spark = ecs__create_entity(ecs);

    directions_to_average[1] =
      scalar_x_vec3(10, random_direction());

    ecs__add_transform(spark, transform, ecs);
    ecs__add_velocity(
      spark,
      vec3__mean(directions_to_average, 2),
      ecs
    );
    // ecs__add_draw_back_faces(spark, ecs);
    ecs__add_draw(
      spark,
      (struct Draw){
        .texture = &TINY_DEBRIS_TEXTURE,
        .mesh = &QUAD,
        .shader = &SOLID_COLOR_SHADER,
        .draw = ecs__draw_mesh
      },
      ecs
    );
    ecs__add_gravity(spark, ecs);
    ecs__add_timeout(
      spark,
      (struct Timeout){
        .age = 0,
        .limit = 0.5,
        .on_timeout = destroy_spark
      },
      ecs
    );
  }
}