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

void ecs__add_transform(
  EntityId id,
  struct Transform t,
  struct ECS *const ecs
) {
  ecs->entities[id].config += c_TRANSFORM;
  ecs->entities[id].transform = t;
}

void ecs__add_draw_mesh(
  EntityId id,
  struct Draw drw,
  struct ECS *const ecs
) {
  ecs->entities[id].config += c_DRAW_MESH;
  ecs->entities[id].draw = drw;
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