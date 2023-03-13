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
  ECS *const ecs
);

void ecs__remove_player_controller(
  EntityId id,
  ECS *const ecs
);

void ecs__add_transform(
  EntityId id,
  Transform t,
  ECS *const ecs
);

void ecs__add_parent_relationship(
  EntityId parent,
  EntityId child,
  ECS *const ecs
);

void ecs__remove_parent_relationship(
  EntityId child,
  ECS *const ecs
) ;

void ecs__add_alpha_effect(
  EntityId id,
  ECS *const ecs
);

void ecs__add_draw(
  EntityId id,
  Draw draw,
  ECS *const ecs
);

void ecs__remove_draw(
  EntityId id,
  ECS *const ecs
);

void ecs__add_draw_back_faces(
  EntityId id,
  ECS *const ecs
);

void ecs__add_gravity(
  EntityId id,
  ECS *const ecs
);

void ecs__remove_gravity(
  EntityId id,
  ECS *const ecs
);

void ecs__add_velocity(
  EntityId id,
  Vec3 velocity,
  ECS *const ecs
);

void ecs__remove_velocity(
  EntityId id,
  ECS *const ecs
);

void ecs__add_timeout(
  EntityId id,
  Timeout t,
  ECS *const ecs
);

void ecs__remove_timeout(
  EntityId id,
  ECS *const ecs
);

void ecs__add_repeat(
  EntityId id,
  Repeat repeat,
  ECS *const ecs
);

void ecs__remove_repeat(
  EntityId id,
  ECS *const ecs
);

void ecs__add_vec3lerp(
  EntityId id,
  Vec3Lerp vec3lerp,
  ECS *const ecs
);

void ecs__remove_vec3lerp(
  EntityId id,
  ECS *const ecs
);

void ecs__add_uv_scroll(
  EntityId id,
  ScrollUV scroll_uv,
  ECS *const ecs
);

void ecs__remove_uv_scroll(
  EntityId id,
  ECS *const ecs
);

void ecs__add_revolve_lerp(
  EntityId id,
  RevolveLerp rev_lerp,
  ECS *const ecs
);

void ecs__remove_revolve_lerp(
  EntityId id,
  ECS *const ecs
);

void ecs__add_rotation_lerp(
  EntityId id,
  RotationLerp rotation_lerp,
  ECS *const ecs
);

void ecs__remove_rotation_lerp(
  EntityId id,
  ECS *const ecs
);

void ecs__add_look_at_center(
  EntityId id,
  ECS *const ecs
);

void ecs__remove_look_at_center(
  EntityId id,
  ECS *const ecs
);

void ecs__add_weapons(
  EntityId id,
  Weapons weapons,
  ECS *const ecs
);

void ecs__remove_weapons(
  EntityId id,
  ECS *const ecs  
);

void ecs__add_radius_collider(
  EntityId id,
  float radius,
  ECS *const ecs
);

void ecs__remove_radius_collider(
  EntityId id,
  ECS *const ecs
);

void ecs__add_damagable(
  EntityId id,
  void (*on_hit_by_damager)(
    EntityId id,
    EntityId other,
    ECS *const ecs
  ),
  ECS *const ecs
);

void ecs__remove_damagable(
  EntityId id,
  ECS *const ecs
);

void ecs__add_damager(
  EntityId id,
  ECS *const ecs
);

void ecs__remove_damager(
  EntityId id,
  ECS *const ecs
);

void ecs__add_pickupable(
  EntityId id,
  void (*on_picked_up)(
    EntityId pickupable,
    EntityId pickuper,
    ECS *const ecs
  ),
  ECS *const ecs
);

void ecs__remove_pickupable(
  EntityId id,
  ECS *const ecs
);

#endif