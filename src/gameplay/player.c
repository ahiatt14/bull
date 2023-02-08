#include <stdio.h>
#include <math.h>

#include "tail.h"

#include "ecs.h"

#include "player.h"
#include "bull_math.h"
#include "constants.h"
#include "tail_helpers.h"

#include "bird_mesh.h"
#include "default_vert.h"

void player__copy_assets_to_gpu(
  struct GPU const *const gpu
) {
  gpu->copy_static_mesh_to_gpu(&BIRD_MESH);
}

EntityId create_player(
  struct Vec3 position,
  void (*fire_lvl0_cannon)(
    EntityId player,
    Seconds remainder,
    struct ECS *const ecs
  ),
  struct ECS *const ecs
) {

  EntityId player = ecs__create_entity(ecs);

  ecs__add_player_controller(player, ecs);
  ecs__add_look_at_center(player, ecs);
  ecs__add_velocity(player, (struct Vec3){0}, ecs);
  ecs__add_weapons(
    player,
    (struct Weapons){
      .primary = fire_lvl0_cannon,
      .primary_autofire_interval = LVL0_CANNON_AUTOFIRE_INTERVAL
    },
    ecs
  );
  ecs__add_transform(
    player,
    (struct Transform){
      .position = position,
      .scale = 1
    },
    ecs
  );
  ecs__add_draw(
    player,
    (struct Draw){
      .mesh = &BIRD_MESH,
      .texture = NULL,
      .shader = &SOLID_COLOR_SHADER
    },
    ecs
  );

  return player;
}