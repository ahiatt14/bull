#include "tail.h"

#include "ecs_types.h"
#include "ecs_entities.h"

EntityId ecs__create_entity(
  struct ECS *const ecs
) {
  EntityId id = ecs->count++;
  ecs->entities[id].config = 0;
  return id;
}

void ecs__destroy_entity(
  EntityId id,
  struct ECS *const ecs
) {
  if (id == ecs->count - 1) {
    ecs->count--;
  } else {
    ecs->entities[id] = ecs->entities[--ecs->count];
  }
}