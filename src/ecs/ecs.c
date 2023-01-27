#include <stdio.h>

#include "tail.h"

#include "ecs.h"

#include "constants.h"
#include "tail_helpers.h"

static struct entities state;

static uint_fast8_t lacks_configuration(
  uint_fast16_t necessary_configuration,
  uint_fast16_t entity_configuration
) {
  uint_fast16_t anded = necessary_configuration & entity_configuration;
  return (anded == necessary_configuration) ? 0 : 1;
}

static uint_fast8_t has_component(
  uint_fast16_t necessary_component,
  uint_fast16_t entity_configuration
) {
  uint_fast16_t anded = necessary_component & entity_configuration;
  return (anded == necessary_component) ? 1 : 0;
}

uint_fast16_t get_active_entity_count() {
  return state._active_count;
}

struct entity* ecs__get_entity(
  entity_id id
) {
  return &(state._entities[id]);
}

entity_id ecs__create_entity() {
  entity_id id = state._active_count++;
  state._entities[id].config = 0;
  return id;
}

void ecs__destroy_entity(
  entity_id id
) {
  if (id == state._active_count - 1) {
    state._active_count--;
  } else {
    state._entities[id] = state._entities[--state._active_count];
  }
}

void ecs__add_transform(
  entity_id id,
  struct transform t
) {
  state._entities[id].config += c_TRANSFORM;
  state._entities[id].transform = t;
}

void ecs__add_draw_mesh(
  entity_id id,
  struct draw drw
) {
  state._entities[id].config += c_DRAW_MESH;
  state._entities[id].draw = drw;
}

void ecs__add_velocity(
  entity_id id,
  struct vec3 velocity
) {
  state._entities[id].config += c_VELOCITY;
  state._entities[id].velocity = velocity;
}

void ecs__remove_velocity(
  entity_id id
) {
  state._entities[id].config -= c_VELOCITY;
}

void ecs__add_timeout(
  entity_id id,
  struct timeout t
) {
  state._entities[id].config += c_TIMEOUT;
  state._entities[id].timeout = t;
}

void ecs__remove_timeout(
  entity_id id
) {
  state._entities[id].config -= c_TIMEOUT;
}

void ecs__move(
  struct gametime time
) {

  for (entity_id id = 0; id < state._active_count; id++) {

    if (lacks_configuration(
      c_TRANSFORM | c_VELOCITY,
      state._entities[id].config
    )) continue;

    state._entities[id].transform.position = vec3_plus_vec3(
      state._entities[id].transform.position,
      scalar_x_vec3(time.delta, state._entities[id].velocity)
    );
  }
}

void ecs__timeout(
  struct gametime time
) {
  
  for (entity_id id = 0; id < state._active_count; id++) {

    if (lacks_configuration(
      c_TIMEOUT,
      state._entities[id].config
    )) continue;

    state._entities[id].timeout.seconds_since_activation += time.delta;
    if (
      state._entities[id].timeout.seconds_since_activation >=
      state._entities[id].timeout.limit_in_seconds
    ) state._entities[id].timeout.on_timeout(id);
  }
}

void ecs__scroll_uvs(
  struct gametime time
) {

  for (entity_id id = 0; id < state._active_count; id++) {

    if (lacks_configuration(
      c_DRAW_MESH | c_UV_SCROLL,
      state._entities[id].config
    )) continue;

    for (
      uint_fast16_t i = 0;
      i < state._entities[id].draw.mesh->vertices_length;
      i++
    ) {
      state._entities[id].draw.mesh->vertices[i].uv.x +=
        state._entities[id].draw.uv_scroll_speed.x * time.delta;
      state._entities[id].draw.mesh->vertices[i].uv.y +=
        state._entities[id].draw.uv_scroll_speed.y * time.delta;
    }
  }
}

void ecs__draw(
  struct gametime time,
  struct camera const *const cam,
  struct gpu_api const *const gpu
) {

  for (entity_id id = 0; id < state._active_count; id++) {

    if (
      lacks_configuration(c_DRAW_BILLBOARD, state._entities[id].config) &&
      lacks_configuration(c_DRAW_MESH, state._entities[id].config)
    ) continue;

    struct shader *shad = state._entities[id].draw.shader;

    gpu->select_shader(shad);

    if (state._entities[id].draw.texture != NULL)
      gpu->select_texture(state._entities[id].draw.texture);

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
      state._entities[id].config
    )) {

      m4x4__translation(
        state._entities[id].transform.position,
        &local_to_world
      );
      gpu->set_shader_float(
        shad,
        "scale",
        state._entities[id].transform.scale
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
        &state._entities[id].transform,
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

      gpu->draw_mesh(state._entities[id].draw.mesh);
    }
  }
}