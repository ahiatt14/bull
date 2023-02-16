#include <stdlib.h>
#include <stdio.h>

#include "tail.h"
#include "ecs.h"

#include "sparks.h"

#include "constants.h"

#include "spark_geo.h"
#include "spark_frag.h"
#include "spark_vert.h"

static struct Shader spark_shader;

void sparks__copy_assets_to_gpu(
  struct GPU const *const gpu
) {
  spark_shader.frag_src = SPARK_FRAG_SRC;
  spark_shader.geo_src = SPARK_GEO_SRC;
  spark_shader.vert_src = SPARK_VERT_SRC;
  gpu->copy_shader_to_gpu(&spark_shader);
}

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

static void draw_spark(
  struct GameTime time,
  struct Camera const *const camera,
  struct GPU const *const gpu,
  EntityId spark,
  struct ECS const *const ecs
) {

  static struct M4x4 model;
  static struct Shader *shader;
  shader = ecs->entities[spark].draw.shader;
 
  m4x4__translation(ecs->entities[spark].transform.position, &model);
  gpu->set_shader_m4x4(shader, "model", &model);
  gpu->set_shader_vec3(shader, "velocity", ecs->entities[spark].velocity);

  gpu->draw_points(&POINT);
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
        .texture = NULL,
        .mesh = NULL,
        .shader = &spark_shader,
        .draw = draw_spark
      },
      ecs
    );
    ecs__add_gravity(spark, ecs);
    ecs__add_timeout(
      spark,
      (struct Timeout){
        .age = 0,
        .limit = 6,
        .on_timeout = destroy_spark
      },
      ecs
    );
  }
}