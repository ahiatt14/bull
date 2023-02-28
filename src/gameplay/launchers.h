#ifndef __BULL_LAUNCHERS__
#define __BULL_LAUNCHERS__

#include "tail.h"

#include "ecs.h"

void launchers__copy_assets_to_gpu(
  struct GPU const *const gpu
);

void launchers__init_scene_callbacks(
  void (*on_player_collide_with_radial)(
    EntityId launcher,
    EntityId player,
    struct ECS *const ecs
  ),
  double (*const get_seconds_since_creation)()
);

EntityId create_radial_launcher(
  struct Vec3 position,
  struct ECS *const ecs
);

void create_radial_launcher_spawner(
  struct ECS *const ecs
);

#endif