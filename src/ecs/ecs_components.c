#include "tail.h"

#include "ecs_definitions.h"
#include "ecs_components.h"

void ecs__add_transform(
  EntityId id,
  struct transform t,
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
  struct vec3 velocity,
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