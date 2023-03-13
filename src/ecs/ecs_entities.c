#include "tail.h"

#include "ecs_types.h"
#include "ecs_entities.h"
#include "ecs_component_fns.h"

// HELPER DECS

static void destroy_entity(
  EntityId id,
  ECS *const ecs
);

static void sort_doomed_entity_ids_descending(
  EntityId *ids,
  uint_fast16_t count
);

// PUBLIC API

EntityId ecs__create_entity(
  ECS *const ecs
) {
  EntityId id = ecs->count++;
  ecs->entities[id].config = 0;
  return id;
}

void ecs__mark_for_destruction(
  EntityId id,
  ECS *const ecs
) {
  ecs->doomed_entities[ecs->doomed_entity_count++] = id;
}

void ecs__destroy_marked_entities(
  ECS *const ecs
) {

  sort_doomed_entity_ids_descending(
    ecs->doomed_entities,
    ecs->doomed_entity_count
  );

  for (uint_fast16_t i = 0; i < ecs->doomed_entity_count; i++) {
    destroy_entity(ecs->doomed_entities[i], ecs);
  }
  ecs->doomed_entity_count = 0;
}

// HELPER DEFS

static void destroy_entity(
  EntityId id,
  ECS *const ecs
) {
  if (id == ecs->count - 1) {
    ecs->count--;
  } else {
    ecs->entities[id] = ecs->entities[--ecs->count];
  }
}

static void sort_doomed_entity_ids_descending(
  EntityId *ids,
  uint_fast16_t count
) {
  for (uint_fast16_t i = 0; i < count; i++)
  for (uint_fast16_t j = 0; j < count - 1; j++) {
    if (ids[j] > ids[j + 1]) continue;
    EntityId temp = ids[j];
    ids[j] = ids[j + 1];
    ids[j + 1] = temp;
  }
}