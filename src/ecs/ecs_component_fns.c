#include <string.h>

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
  ECS *const ecs
) {
  if (lacks_components(
    c_PLAYER_CONTROLLER,
    ecs->entities[id].config
  )) ecs->entities[id].config += c_PLAYER_CONTROLLER;
}

void ecs__remove_player_controller(
  EntityId id,
  ECS *const ecs
) {
  if (has_component(
    c_PLAYER_CONTROLLER,
    ecs->entities[id].config
  )) ecs->entities[id].config -= c_PLAYER_CONTROLLER;
}

void ecs__add_transform(
  EntityId id,
  Transform t,
  ECS *const ecs
) {
  if (lacks_components(
    c_TRANSFORM,
    ecs->entities[id].config
  )) ecs->entities[id].config += c_TRANSFORM;
  ecs->entities[id].transform = t;
}

void ecs__add_parent_relationship(
  EntityId parent,
  EntityId child,
  ECS *const ecs
) {

  if (lacks_components(
    c_HAS_PARENT,
    ecs->entities[child].config
  )) ecs->entities[child].config += c_HAS_PARENT;

  if (lacks_components(
    c_HAS_CHILDREN,
    ecs->entities[parent].config
  )) ecs->entities[parent].config += c_HAS_CHILDREN;

  uint_fast8_t original_child_count =
    ecs->entities[parent].hierarchy.child_count;

  ecs->entities[parent].hierarchy.child_count = original_child_count + 1;
  ecs->entities[parent].hierarchy.children[original_child_count] = child;
  ecs->entities[child].hierarchy.parent = parent;
}

void ecs__remove_parent_relationship(
  EntityId child,
  ECS *const ecs
) {

  if (has_component(
    c_HAS_PARENT,
    ecs->entities[child].config
  )) ecs->entities[child].config -= c_HAS_PARENT;

  EntityId parent = ecs->entities[child].hierarchy.parent;

  ecs->entities[parent].hierarchy.child_count--;
  if (ecs->entities[parent].hierarchy.child_count == 0) {
    ecs->entities[parent].config -= c_HAS_CHILDREN;
    return;
  }

  static EntityId remaining_children[MAX_DIRECT_CHILDREN];
  uint_fast8_t remaining_child_count = 0;
  for (
    uint_fast8_t i = 0;
    i < ecs->entities[parent].hierarchy.child_count;
    i++
  ) if (ecs->entities[parent].hierarchy.children[i] != child)
    remaining_children[remaining_child_count++] =
      ecs->entities[parent].hierarchy.children[i];

  memcpy(
    ecs->entities[parent].hierarchy.children,
    remaining_children,
    sizeof(EntityId) * MAX_DIRECT_CHILDREN
  );
}

void ecs__add_point_light_source(
  EntityId id,
  PointLight light,
  ECS *const ecs
) {
  if (lacks_components(
    c_POINT_LIGHT,
    ecs->entities[id].config
  )) ecs->entities[id].config += c_POINT_LIGHT;
  ecs->entities[id].point_light = light;
}

void ecs__remove_point_light_source(
  EntityId id,
  ECS *const ecs
) {
  if (has_component(
    c_POINT_LIGHT,
    ecs->entities[id].config
  )) ecs->entities[id].config -= c_POINT_LIGHT;
}

void ecs__add_alpha_effect(
  EntityId id,
  ECS *const ecs
) {
  if (lacks_components(
    c_ALPHA_EFFECT,
    ecs->entities[id].config
  )) ecs->entities[id].config += c_ALPHA_EFFECT;
}

void ecs__add_draw(
  EntityId id,
  Draw draw,
  ECS *const ecs
) {
  if (lacks_components(
    c_DRAW,
    ecs->entities[id].config
  )) ecs->entities[id].config += c_DRAW;
  ecs->entities[id].draw = draw;
}

void ecs__remove_draw(
  EntityId id,
  ECS *const ecs
) {
  if (has_component(
    c_DRAW,
    ecs->entities[id].config
  )) ecs->entities[id].config -= c_DRAW;
}

void ecs__add_draw_back_faces(
  EntityId id,
  ECS *const ecs
) {
  if (lacks_components(
    c_DRAW_BACK_FACES,
    ecs->entities[id].config
  )) ecs->entities[id].config += c_DRAW_BACK_FACES;
}

void ecs__add_gravity(
  EntityId id,
  ECS *const ecs
) {
  if (lacks_components(
    c_GRAVITY,
    ecs->entities[id].config
  )) ecs->entities[id].config += c_GRAVITY;
}

void ecs__remove_gravity(
  EntityId id,
  ECS *const ecs
) {
  if (has_component(
    c_GRAVITY,
    ecs->entities[id].config
  )) ecs->entities[id].config -= c_GRAVITY;
}

void ecs__add_velocity(
  EntityId id,
  Vec3 velocity,
  ECS *const ecs
) {
  if (lacks_components(
    c_VELOCITY,
    ecs->entities[id].config
  )) ecs->entities[id].config += c_VELOCITY;
  ecs->entities[id].velocity = velocity;
}

void ecs__remove_velocity(
  EntityId id,
  ECS *const ecs
) {
  if (has_component(
    c_VELOCITY,
    ecs->entities[id].config
  )) ecs->entities[id].config -= c_VELOCITY;
}

void ecs__add_timeout(
  EntityId id,
  Timeout t,
  ECS *const ecs
) {
  if (lacks_components(
    c_TIMEOUT,
    ecs->entities[id].config
  )) ecs->entities[id].config += c_TIMEOUT;
  ecs->entities[id].timeout = t;
}

void ecs__remove_timeout(
  EntityId id,
  ECS *const ecs
) {
  if (has_component(
    c_TIMEOUT,
    ecs->entities[id].config
  )) ecs->entities[id].config -= c_TIMEOUT;
}

void ecs__add_repeat(
  EntityId id,
  Repeat repeat,
  ECS *const ecs
) {
  if (lacks_components(
    c_REPEAT,
    ecs->entities[id].config
  )) ecs->entities[id].config += c_REPEAT;
  ecs->entities[id].repeat = repeat;
}

void ecs__remove_repeat(
  EntityId id,
  ECS *const ecs
) {
  if (has_component(
    c_REPEAT,
    ecs->entities[id].config
  )) ecs->entities[id].config -= c_REPEAT;
}

void ecs__add_vec3lerp(
  EntityId id,
  Vec3Lerp vec3lerp,
  ECS *const ecs
) {
  if (lacks_components(
    c_VEC3_LERP,
    ecs->entities[id].config
  )) ecs->entities[id].config += c_VEC3_LERP;
  ecs->entities[id].vec3lerp = vec3lerp;
}

void ecs__remove_vec3lerp(
  EntityId id,
  ECS *const ecs
) {
  if (has_component(
    c_VEC3_LERP,
    ecs->entities[id].config
  )) ecs->entities[id].config -= c_VEC3_LERP;
}

void ecs__add_uv_scroll(
  EntityId id,
  ScrollUV scroll_uv,
  ECS *const ecs
) {
  if (lacks_components(
    c_UV_SCROLL,
    ecs->entities[id].config
  )) ecs->entities[id].config += c_UV_SCROLL;
  ecs->entities[id].scroll_uv = scroll_uv;
}

void ecs__remove_uv_scroll(
  EntityId id,
  ECS *const ecs
) {
  if (has_component(
    c_UV_SCROLL,
    ecs->entities[id].config
  )) ecs->entities[id].config -= c_UV_SCROLL;
}

void ecs__add_revolve_lerp(
  EntityId id,
  RevolveLerp revolve_lerp,
  ECS *const ecs
) {
  if (lacks_components(
    c_REVOLVE_LERP,
    ecs->entities[id].config
  )) ecs->entities[id].config += c_REVOLVE_LERP;
  ecs->entities[id].revolve_lerp = revolve_lerp;
}

void ecs__remove_revolve_lerp(
  EntityId id,
  ECS *const ecs
) {
  if (has_component(
    c_REVOLVE_LERP,
    ecs->entities[id].config
  )) ecs->entities[id].config -= c_REVOLVE_LERP;
}

void ecs__add_rotation_lerp(
  EntityId id,
  RotationLerp rotation_lerp,
  ECS *const ecs
) {
  if (lacks_components(
    c_ROTATION_LERP,
    ecs->entities[id].config
  )) ecs->entities[id].config += c_ROTATION_LERP;
  ecs->entities[id].rotation_lerp = rotation_lerp;
}

void ecs__remove_rotation_lerp(
  EntityId id,
  ECS *const ecs
) {
  if (has_component(
    c_ROTATION_LERP,
    ecs->entities[id].config
  )) ecs->entities[id].config -= c_ROTATION_LERP;
}

void ecs__add_look_at_center(
  EntityId id,
  ECS *const ecs
) {
  if (lacks_components(
    c_LOOK_AT_CENTER,
    ecs->entities[id].config
  )) ecs->entities[id].config += c_LOOK_AT_CENTER;
}

void ecs__remove_look_at_center(
  EntityId id,
  ECS *const ecs
) {
  if (has_component(
    c_LOOK_AT_CENTER,
    ecs->entities[id].config
  )) ecs->entities[id].config -= c_LOOK_AT_CENTER;
}

void ecs__add_weapons(
  EntityId id,
  Weapons weapons,
  ECS *const ecs
) {
  if (lacks_components(
    c_WEAPONS,
    ecs->entities[id].config
  )) ecs->entities[id].config += c_WEAPONS;
  ecs->entities[id].weapons = weapons;
}

void ecs__remove_weapons(
  EntityId id,
  ECS *const ecs  
) {
  if (has_component(
    c_WEAPONS,
    ecs->entities[id].config
  )) ecs->entities[id].config -= c_WEAPONS;
}

void ecs__add_radius_collider(
  EntityId id,
  float radius,
  ECS *const ecs
) {
  if (lacks_components(
    c_RADIUS_COLLIDER,
    ecs->entities[id].config
  )) ecs->entities[id].config += c_RADIUS_COLLIDER;
  ecs->entities[id].radius = radius;
}

void ecs__remove_radius_collider(
  EntityId id,
  ECS *const ecs
) {
  if (has_component(
    c_RADIUS_COLLIDER,
    ecs->entities[id].config
  )) ecs->entities[id].config -= c_RADIUS_COLLIDER;
}

void ecs__add_damagable(
  EntityId id,
  void (*on_hit_by_damager)(
    EntityId collidee,
    EntityId collider,
    ECS *const ecs
  ),
  ECS *const ecs
) {
  if (lacks_components(
    c_DAMAGABLE,
    ecs->entities[id].config
  )) ecs->entities[id].config += c_DAMAGABLE;
  ecs->entities[id].on_hit_by_damager = on_hit_by_damager;
}

void ecs__remove_damagable(
  EntityId id,
  ECS *const ecs
) {
  if (has_component(
    c_DAMAGABLE,
    ecs->entities[id].config
  )) ecs->entities[id].config -= c_DAMAGABLE;
}

void ecs__add_damager(
  EntityId id,
  ECS *const ecs
) {
  if (lacks_components(
    c_DAMAGER,
    ecs->entities[id].config
  )) ecs->entities[id].config += c_DAMAGER;
}

void ecs__remove_damager(
  EntityId id,
  ECS *const ecs
) {
  if (has_component(
    c_DAMAGER,
    ecs->entities[id].config
  )) ecs->entities[id].config -= c_DAMAGER;
}

void ecs__add_pickupable(
  EntityId id,
  void (*on_picked_up)(
    EntityId pickupable,
    EntityId pickuper,
    ECS *const ecs
  ),
  ECS *const ecs
) {
  if (lacks_components(
    c_PICKUPABLE,
    ecs->entities[id].config
  )) ecs->entities[id].config += c_PICKUPABLE;
  ecs->entities[id].on_picked_up = on_picked_up;
}

void ecs__remove_pickupable(
  EntityId id,
  ECS *const ecs
) {
  if (has_component(
    c_PICKUPABLE,
    ecs->entities[id].config
  )) ecs->entities[id].config -= c_PICKUPABLE;
}