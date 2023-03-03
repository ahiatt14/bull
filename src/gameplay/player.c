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
#include "firing_guide_frag.h"

Shader player_shader;
static Shader guide_shader;

void player__copy_assets_to_gpu(
  GPU const *const gpu
) {

  player_shader.frag_src = MOUNTAIN_FRAG_SRC;
  player_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&player_shader);

  gpu->copy_static_mesh_to_gpu(&REX_MESH);

  guide_shader.frag_src = FIRING_GUIDE_FRAG_SRC;
  guide_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&guide_shader);
}

EntityId create_player(
  Vec3 position,
  void (*fire_lvl0_cannon)(
    EntityId player,
    Seconds remainder,
    ECS *const ecs
  ),
  ECS *const ecs
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
    (Weapons){
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
    (Draw){
      .mesh = &REX_MESH,
      .textures = DARK_RUST_TEXTURE,
      .shader = &player_shader,
      .draw = ecs__draw_mesh
    },
    ecs
  );

  return player;
}

static void draw_firing_guide(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  EntityId id,
  ECS const *const ecs
) {

  static Entity const *guide;
  static Entity const *player;
  guide = &ecs->entities[id];
  player = &ecs->entities[PLAYER_ID];

  static M4x4 model;
  static Shader *shader;
  shader = guide->draw.shader;

  m4x4__scaling(
    guide->transform.scale,
    &model
  );

  gpu->set_shader_m4x4(shader, "model", &model);

  gpu->set_shader_float(shader, "arena_radius_worldspace", ARENA_RADIUS);
  gpu->set_shader_vec3(
    shader,
    "player_pos_worldspace",
    player->transform.position
  );

  gpu->draw_mesh(guide->draw.mesh);
}

EntityId create_firing_guide(
  ECS *const ecs
) {

  EntityId guide = ecs__create_entity(ecs);

  ecs__add_transform(
    guide,
    (Transform){
      .scale = 25
    },
    ecs
  );
  ecs__add_draw(
    guide,
    (Draw){
      .mesh = &QUAD,
      .shader = &guide_shader,
      .textures = 0,
      .draw = draw_firing_guide
    },
    ecs
  );
  ecs__add_alpha_effect(guide, ecs);

  return guide;
}