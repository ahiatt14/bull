#ifndef __BULL_LAUNCHERS__
#define __BULL_LAUNCHERS__

#include "tail.h"

#include "ecs.h"

void launchers__init_scene_callbacks(
  void (*on_player_collide_with_radial)(
    EntityId launcher,
    EntityId player,
    ECS *const ecs
  ),
  double (*const get_seconds_since_creation)()
);

EntityId create_radial_launcher(
  Vec3 position,
  ECS *const ecs
);

void launch_entity_radially(
  Vec3 start_position,
  Vec3 end_position,
  Seconds travel_time,
  void (*on_reach_destination)(
    EntityId entity,
    Seconds remainder,
    ECS *const ecs
  ),
  EntityId entity,
  ECS *const ecs
);

void create_radial_launcher_spawner(
  ECS *const ecs
);

#endif