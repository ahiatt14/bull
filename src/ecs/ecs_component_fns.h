#ifndef __BULL_ECS_COMPONENT_FNS__
#define __BULL_ECS_COMPONENT_FNS__

#include "tail.h"

#include "ecs_types.h"

uint_fast8_t lacks_components(
  ComponentConfig necessary_configuration,
  ComponentConfig entity_configuration
);

uint_fast8_t has_component(
  ComponentConfig necessary_component,
  ComponentConfig entity_configuration
);

void ecs__add_player_controller(
  EntityId id,
  struct ECS *const ecs
);

void ecs__remove_player_controller(
  EntityId id,
  struct ECS *const ecs
);

void ecs__add_transform(
  EntityId id,
  Transform t,
  struct ECS *const ecs
);

void ecs__add_alpha_effect(
  EntityId id,
  struct ECS *const ecs
);

void ecs__add_draw(
  EntityId id,
  struct Draw draw,
  struct ECS *const ecs
);

void ecs__remove_draw(
  EntityId id,
  struct ECS *const ecs
);

void ecs__add_draw_back_faces(
  EntityId id,
  struct ECS *const ecs
);

void ecs__add_gravity(
  EntityId id,
  struct ECS *const ecs
);

void ecs__remove_gravity(
  EntityId id,
  struct ECS *const ecs
);

void ecs__add_velocity(
  EntityId id,
  Vec3 velocity,
  struct ECS *const ecs
);

void ecs__remove_velocity(
  EntityId id,
  struct ECS *const ecs
);

void ecs__add_timeout(
  EntityId id,
  struct Timeout t,
  struct ECS *const ecs
);

void ecs__remove_timeout(
  EntityId id,
  struct ECS *const ecs
);

void ecs__add_repeat(
  EntityId id,
  struct Repeat repeat,
  struct ECS *const ecs
);

void ecs__remove_repeat(
  EntityId id,
  struct ECS *const ecs
);

void ecs__add_vec3lerp(
  EntityId id,
  struct Vec3Lerp vec3lerp,
  struct ECS *const ecs
);

void ecs__remove_vec3lerp(
  EntityId id,
  struct ECS *const ecs
);

void ecs__add_uv_scroll(
  EntityId id,
  struct ScrollUV scroll_uv,
  struct ECS *const ecs
);

void ecs__remove_uv_scroll(
  EntityId id,
  struct ECS *const ecs
);

void ecs__add_revolve_lerp(
  EntityId id,
  struct RevolveLerp rev_lerp,
  struct ECS *const ecs
);

void ecs__remove_revolve_lerp(
  EntityId id,
  struct ECS *const ecs
);

void ecs__add_rotation_lerp(
  EntityId id,
  struct RotationLerp rotation_lerp,
  struct ECS *const ecs
);

void ecs__remove_rotation_lerp(
  EntityId id,
  struct ECS *const ecs
);

void ecs__add_look_at_center(
  EntityId id,
  struct ECS *const ecs
);

void ecs__remove_look_at_center(
  EntityId id,
  struct ECS *const ecs
);

void ecs__add_weapons(
  EntityId id,
  struct Weapons weapons,
  struct ECS *const ecs
);

void ecs__remove_weapons(
  EntityId id,
  struct ECS *const ecs  
);

void ecs__add_radius_collider(
  EntityId id,
  float radius,
  struct ECS *const ecs
);

void ecs__remove_radius_collider(
  EntityId id,
  struct ECS *const ecs
);

void ecs__add_damagable(
  EntityId id,
  void (*on_hit_by_damager)(
    EntityId id,
    EntityId other,
    struct ECS *const ecs
  ),
  struct ECS *const ecs
);

void ecs__remove_damagable(
  EntityId id,
  struct ECS *const ecs
);

void ecs__add_damager(
  EntityId id,
  struct ECS *const ecs
);

void ecs__remove_damager(
  EntityId id,
  struct ECS *const ecs
);

void ecs__add_pickupable(
  EntityId id,
  void (*on_picked_up)(
    EntityId pickupable,
    EntityId pickuper,
    struct ECS *const ecs
  ),
  struct ECS *const ecs
);

void ecs__remove_pickupable(
  EntityId id,
  struct ECS *const ecs
);

#endif