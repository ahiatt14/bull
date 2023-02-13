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

void ecs__mark_for_destruction(
  EntityId id,
  struct ECS *const ecs
) {
  ecs->entities_to_destroy[ecs->entities_to_destroy_count++] = id;
}

void ecs__destroy_marked_entities(
  struct ECS *const ecs
) {
  EntityId id_to_destroy;
  for (uint_fast16_t i = 0; i < ecs->entities_to_destroy_count; i++) {

    id_to_destroy = ecs->entities_to_destroy[i];

    if (id_to_destroy == ecs->count - 1) {
      ecs->count--;
    } else {
      ecs->entities[id_to_destroy] = ecs->entities[--ecs->count];
    }
  }
  ecs->entities_to_destroy_count = 0;
}