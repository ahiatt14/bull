#include "tail.h"

#include "ecs_types.h"
#include "ecs_component_fns.h"

uint_fast8_t lacks_components(
  ComponentConfig necessary_configuration,
  ComponentConfig entity_configuration
) {
  uint_fast32_t anded = necessary_configuration & entity_configuration;
  return (anded == necessary_configuration) ? 0 : 1;
}

uint_fast8_t has_component(
  ComponentConfig necessary_component,
  ComponentConfig entity_configuration
) {
  uint_fast32_t anded = necessary_component & entity_configuration;
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
  Transform t,
  struct ECS *const ecs
) {
  ecs->entities[id].config += c_TRANSFORM;
  ecs->entities[id].transform = t;
}

void ecs__add_alpha_effect(
  EntityId id,
  struct ECS *const ecs
) {
  ecs->entities[id].config += c_ALPHA_EFFECT;
}

void ecs__add_draw(
  EntityId id,
  struct Draw draw,
  struct ECS *const ecs
) {
  ecs->entities[id].config += c_DRAW;
  ecs->entities[id].draw = draw;
}

void ecs__remove_draw(
  EntityId id,
  struct ECS *const ecs
) {
  ecs->entities[id].config -= c_DRAW;
}

void ecs__add_draw_back_faces(
  EntityId id,
  struct ECS *const ecs
) {
  ecs->entities[id].config += c_DRAW_BACK_FACES;
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
  Vec3 velocity,
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
  struct ScrollUV scroll_uv,
  struct ECS *const ecs
) {
  ecs->entities[id].config += c_UV_SCROLL;
  ecs->entities[id].scroll_uv = scroll_uv;
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

void ecs__add_rotation_lerp(
  EntityId id,
  struct RotationLerp rotation_lerp,
  struct ECS *const ecs
) {
  ecs->entities[id].rotation_lerp = rotation_lerp;
  ecs->entities[id].config += c_ROTATION_LERP;
}

void ecs__remove_rotation_lerp(
  EntityId id,
  struct ECS *const ecs
) {
  ecs->entities[id].config -= c_ROTATION_LERP;
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

void ecs__add_radius_collider(
  EntityId id,
  float radius,
  struct ECS *const ecs
) {
  ecs->entities[id].config += c_RADIUS_COLLIDER;
  ecs->entities[id].radius = radius;
}

void ecs__remove_radius_collider(
  EntityId id,
  struct ECS *const ecs
) {
  ecs->entities[id].config -= c_RADIUS_COLLIDER;
}

void ecs__add_damagable(
  EntityId id,
  void (*on_hit_by_damager)(
    EntityId collidee,
    EntityId collider,
    struct ECS *const ecs
  ),
  struct ECS *const ecs
) {
  ecs->entities[id].config += c_DAMAGABLE;
  ecs->entities[id].on_hit_by_damager = on_hit_by_damager;
}

void ecs__remove_damagable(
  EntityId id,
  struct ECS *const ecs
) {
  ecs->entities[id].config -= c_DAMAGABLE;
}

void ecs__add_damager(
  EntityId id,
  struct ECS *const ecs
) {
  ecs->entities[id].config += c_DAMAGER;
}

void ecs__remove_damager(
  EntityId id,
  struct ECS *const ecs
) {
  ecs->entities[id].config -= c_DAMAGER;
}

void ecs__add_pickupable(
  EntityId id,
  void (*on_picked_up)(
    EntityId pickupable,
    EntityId pickuper,
    struct ECS *const ecs
  ),
  struct ECS *const ecs
) {
  ecs->entities[id].config += c_PICKUPABLE;
  ecs->entities[id].on_picked_up = on_picked_up;
}

void ecs__remove_pickupable(
  EntityId id,
  struct ECS *const ecs
) {
  ecs->entities[id].config -= c_PICKUPABLE;
}