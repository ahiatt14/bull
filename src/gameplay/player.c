#include <stdio.h>
#include <math.h>

#include "tail.h"

#include "ecs.h"

#include "player.h"

#include "assets.h"
#include "bull_math.h"
#include "constants.h"
#include "tail_helpers.h"

#include "rex_mesh.h"
#include "default_vert.h"
#include "mountain_frag.h"

Shader player_shader;

void player__copy_assets_to_gpu(
  GPU const *const gpu
) {

  player_shader.frag_src = MOUNTAIN_FRAG_SRC;
  player_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&player_shader);

  gpu->copy_static_mesh_to_gpu(&REX_MESH);
}

EntityId create_player(
  Vec3 position,
  void (*fire_lvl0_cannon)(
    EntityId player,
    Seconds remainder,
    struct ECS *const ecs
  ),
  struct ECS *const ecs
) {

  // TODO: for now we are relying on a known constant player id of 0
  // this is only true when the player is the first entity created.
  // once the ecs unit controller system can take a controller object
  // then we can truly treat the player as any other entity
  EntityId player = ecs__create_entity(ecs);

  ecs__add_player_controller(player, ecs);
  ecs__add_look_at_center(player, ecs);
  ecs__add_velocity(player, (Vec3){0}, ecs);
  ecs__add_radius_collider(player, 0.2f, ecs);
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
    (Transform){
      .position = position,
      .scale = 0.25f
    },
    ecs
  );
  ecs__add_draw(
    player,
    (struct Draw){
      .mesh = &REX_MESH,
      .textures = DARK_RUST_TEXTURE,
      .shader = &player_shader,
      .draw = ecs__draw_mesh
    },
    ecs
  );

  return player;
}