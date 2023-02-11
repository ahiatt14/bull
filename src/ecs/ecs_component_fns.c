#include "tail.h"

#include "ecs_types.h"
#include "ecs_component_fns.h"

uint_fast8_t lacks_configuration(
  uint_fast16_t necessary_configuration,
  uint_fast16_t entity_configuration
) {
  uint_fast16_t anded = necessary_configuration & entity_configuration;
  return (anded == necessary_configuration) ? 0 : 1;
}

uint_fast8_t has_component(
  uint_fast16_t necessary_component,
  uint_fast16_t entity_configuration
) {
  uint_fast16_t anded = necessary_component & entity_configuration;
  return (anded == necessary_component) ? 1 : 0;
}

void ecs__add_player_controller(
  EntityId id,
  struct ECS *const ecs
) {
  ecs->entities[id].config += c_PLAYER_CONTROLLER;
}

void ecs__remove_player_controller(
  EntityId id,
  struct ECS *const ecs
) {
  ecs->entities[id].config -= c_PLAYER_CONTROLLER;
}

void ecs__add_transform(
  EntityId id,
  struct Transform t,
  struct ECS *const ecs
) {
  ecs->entities[id].config += c_TRANSFORM;
  ecs->entities[id].transform = t;
}

void ecs__add_draw(
  EntityId id,
  struct Draw draw,
  struct ECS *const ecs
) {
  ecs->entities[id].config += c_DRAW_MESH;
  ecs->entities[id].draw.texture = draw.texture;
  ecs->entities[id].draw.shader = draw.shader;
  ecs->entities[id].draw.mesh = draw.mesh;
}

void ecs__remove_draw(
  EntityId id,
  struct ECS *const ecs
) {
  ecs->entities[id].config -= c_DRAW_MESH;
}

void ecs__add_draw_billboard(
  EntityId id,
  struct Draw draw,
  struct ECS *const ecs
) {
  ecs->entities[id].config += c_DRAW_BILLBOARD;
  ecs->entities[id].draw = draw;
}

void ecs__remove_draw_billboard(
  EntityId id,
  struct ECS *const ecs
) {
  ecs->entities[id].config -= c_DRAW_BILLBOARD;
}

void ecs__add_gravity(
  EntityId id,
  struct ECS *const ecs
) {
  ecs->entities[id].config += c_GRAVITY;
}

void ecs__remove_gravity(
  EntityId id,
  struct ECS *const ecs
) {
  ecs->entities[id].config -= c_GRAVITY;
}

void ecs__add_velocity(
  EntityId id,
  struct Vec3 velocity,
  struct ECS *const ecs
) {
  ecs->entities[id].config += c_VELOCITY;
  ecs->entities[id].velocity = velocity;
}

void ecs__remove_velocity(
  EntityId id,
  struct ECS *const ecs
) {
  ecs->entities[id].config -= c_VELOCITY;
}

void ecs__add_timeout(
  EntityId id,
  struct Timeout t,
  struct ECS *const ecs
) {
  ecs->entities[id].config += c_TIMEOUT;
  ecs->entities[id].timeout = t;
}

void ecs__remove_timeout(
  EntityId id,
  struct ECS *const ecs
) {
  ecs->entities[id].config -= c_TIMEOUT;
}

void ecs__add_repeat(
  EntityId id,
  struct Repeat repeat,
  struct ECS *const ecs
) {
  ecs->entities[id].config += c_REPEAT;
  ecs->entities[id].repeat = repeat;
}

void ecs__remove_repeat(
  EntityId id,
  struct ECS *const ecs
) {
  ecs->entities[id].config -= c_REPEAT;
}

void ecs__add_vec3lerp(
  EntityId id,
  struct Vec3Lerp vec3lerp,
  struct ECS *const ecs
) {
  ecs->entities[id].config += c_VEC3_LERP;
  ecs->entities[id].vec3lerp = vec3lerp;
}

void ecs__remove_vec3lerp(
  EntityId id,
  struct ECS *const ecs
) {
  ecs->entities[id].config -= c_VEC3_LERP;
}

void ecs__add_uv_scroll(
  EntityId id,
  struct Vec2 speed,
  struct ECS *const ecs
) {
  ecs->entities[id].config += c_UV_SCROLL;
  ecs->entities[id].draw.uv_scroll_total = (struct Vec2){0};
  ecs->entities[id].draw.uv_scroll_speed = speed;
}

void ecs__remove_uv_scroll(
  EntityId id,
  struct ECS *const ecs
) {
  ecs->entities[id].config -= c_UV_SCROLL;
}

void ecs__add_revolve_lerp(
  EntityId id,
  struct RevolveLerp revolve_lerp,
  struct ECS *const ecs
) {
  ecs->entities[id].config += c_REVOLVE_LERP;
  ecs->entities[id].revolve_lerp = revolve_lerp;
}

void ecs__remove_revolve_lerp(
  EntityId id,
  struct ECS *const ecs
) {
  ecs->entities[id].config -= c_REVOLVE_LERP;
}

void ecs__add_look_at_center(
  EntityId id,
  struct ECS *const ecs
) {
  ecs->entities[id].config += c_LOOK_AT_CENTER;
}

void ecs__remove_look_at_center(
  EntityId id,
  struct ECS *const ecs
) {
  ecs->entities[id].config -= c_LOOK_AT_CENTER;
}

void ecs__add_weapons(
  EntityId id,
  struct Weapons weapons,
  struct ECS *const ecs
) {
  ecs->entities[id].weapons = weapons;
  ecs->entities[id].config += c_WEAPONS;
}

void ecs__remove_weapons(
  EntityId id,
  struct ECS *const ecs  
) {
  ecs->entities[id].config -= c_WEAPONS;
}