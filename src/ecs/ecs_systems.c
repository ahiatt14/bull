#include "tail.h"

#include "ecs.h"
#include "ecs_systems.h"
#include "ecs_components.h"

#include "constants.h"
#include "bull_math.h"
#include "tail_helpers.h"

void ecs__move(
  struct GameTime time,
  struct ECS *const ecs
) {

  for (EntityId id = 0; id < ecs->count; id++) {

    if (lacks_configuration(
      c_TRANSFORM | c_VELOCITY,
      ecs->entities[id].config
    )) continue;

    ecs->entities[id].transform.position = vec3_plus_vec3(
      ecs->entities[id].transform.position,
      scalar_x_vec3(time.delta, ecs->entities[id].velocity)
    );
  }
}

void ecs__timeout(
  struct GameTime time,
  struct ECS *const ecs
) {
  
  for (EntityId id = 0; id < ecs->count; id++) {

    if (lacks_configuration(
      c_TIMEOUT,
      ecs->entities[id].config
    )) continue;

    ecs->entities[id].timeout.seconds_since_activation += time.delta;
    if (
      ecs->entities[id].timeout.seconds_since_activation >=
      ecs->entities[id].timeout.limit_in_seconds
    ) ecs->entities[id].timeout.on_timeout(id);
  }
}

void ecs__scroll_uvs(
  struct GameTime time,
  struct ECS *const ecs
) {

  for (EntityId id = 0; id < ecs->count; id++) {

    if (lacks_configuration(
      c_DRAW_MESH | c_UV_SCROLL,
      ecs->entities[id].config
    )) continue;

    for (
      uint_fast16_t i = 0;
      i < ecs->entities[id].draw.mesh->vertices_length;
      i++
    ) {
      ecs->entities[id].draw.mesh->vertices[i].uv.x +=
        ecs->entities[id].draw.uv_scroll_speed.x * time.delta;
      ecs->entities[id].draw.mesh->vertices[i].uv.y +=
        ecs->entities[id].draw.uv_scroll_speed.y * time.delta;
    }
  }
}

void ecs__draw(
  struct GameTime time,
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct ECS *const ecs
) {

  for (EntityId id = 0; id < ecs->count; id++) {

    if (
      lacks_configuration(c_DRAW_BILLBOARD, ecs->entities[id].config) &&
      lacks_configuration(c_DRAW_MESH, ecs->entities[id].config)
    ) continue;

    struct shader *shad = ecs->entities[id].draw.shader;

    gpu->select_shader(shad);

    if (ecs->entities[id].draw.texture != NULL)
      gpu->select_texture(ecs->entities[id].draw.texture);

    gpu->set_shader_m4x4(
      shad,
      "view",
      &cam->lookat
    );
    gpu->set_shader_m4x4(
      shad,
      "projection",
      &cam->projection
    );
    gpu->set_shader_float(
      shad,
      "sec_since_game_launch",
      time.sec_since_game_launch
    );

    struct m4x4 local_to_world;
    struct m3x3 normals_local_to_world;

    if (has_component(
      c_DRAW_BILLBOARD,
      ecs->entities[id].config
    )) {

      m4x4__translation(
        ecs->entities[id].transform.position,
        &local_to_world
      );
      gpu->set_shader_float(
        shad,
        "scale",
        ecs->entities[id].transform.scale
      );
      gpu->set_shader_m4x4(
        shad,
        "model",
        &local_to_world
      );

      gpu->draw_points(&POINT); 

    } else {

      space__create_model(
        &WORLDSPACE,
        &ecs->entities[id].transform,
        &local_to_world
      );
      space__create_normals_model(
        &local_to_world,
        &normals_local_to_world
      );

      gpu->set_shader_m3x3(
        shad,
        "normals_model",
        &normals_local_to_world
      );
      gpu->set_shader_m4x4(
        shad,
        "model",
        &local_to_world
      );

      gpu->draw_mesh(ecs->entities[id].draw.mesh);
    }
  }
}