#include <string.h>

#include "tail.h"

#include "ecs_types.h"
#include "ecs_entities.h"
#include "ecs_component_fns.h"

// HELPER DECS

static void destroy_entity(
  EntityId id,
  ECS *const ecs
);

static void sort_ids_descending(
  EntityId *ids,
  uint_fast16_t count
);

static void doom_all_descendents(
  EntityId parent,
  EntityId *doomed_descendents,
  uint_fast8_t *doomed_descendents_count,
  ECS const *const ecs
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

  static EntityId all_doomed_entities[MAX_ENTITIES];
  uint_fast16_t all_doomed_id_count = 0;

  memcpy(
    all_doomed_entities,
    ecs->doomed_entities,
    sizeof(EntityId) * ecs->doomed_entity_count
  );
  all_doomed_id_count = ecs->doomed_entity_count;

  for (uint_fast16_t i = 0; i < ecs->doomed_entity_count; i++) {

    if (has_component(
      c_HAS_PARENT,
      ecs->entities[all_doomed_entities[i]].config
    )) ecs__remove_parent_relationship(all_doomed_entities[i], ecs);
    
    if (has_component(
      c_HAS_CHILDREN,
      ecs->entities[all_doomed_entities[i]].config
    )) doom_all_descendents(
      all_doomed_entities[i],
      all_doomed_entities,
      &all_doomed_id_count,
      ecs
    );
  }
  ecs->doomed_entity_count = 0;

  sort_ids_descending(all_doomed_entities, all_doomed_id_count);

  for (uint_fast16_t i = 0; i < all_doomed_id_count; i++) {
    destroy_entity(all_doomed_entities[i], ecs);
  }
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

// TODO: bubble sort, ez but slow, could replace later
static void sort_ids_descending(
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

static void doom_all_descendents(
  EntityId parent,
  EntityId *doomed_descendents,
  uint_fast8_t *doomed_descendents_count,
  ECS const *const ecs
) {

  if (lacks_components(
    c_HAS_CHILDREN,
    ecs->entities[parent].config
  )) return;

  EntityId child;
  for (
    uint_fast8_t i = 0;
    i < ecs->entities[parent].hierarchy.child_count;
    i++
  ) {
    child = ecs->entities[parent].hierarchy.children[i];
    doomed_descendents[(*doomed_descendents_count)++] = child;
    doom_all_descendents(
      child,
      doomed_descendents,
      doomed_descendents_count,
      ecs
    );
  }
}