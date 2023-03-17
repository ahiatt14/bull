#include <stdio.h>
#include <math.h>

#include "tail.h"

#include "ecs.h"

#include "player.h"

#include "assets.h"
#include "bull_math.h"
#include "constants.h"
#include "tail_helpers.h"

#include "player_body_mesh.h"
#include "player_left_leg_mesh.h"
#include "player_right_leg_mesh.h"

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

  gpu->copy_static_mesh_to_gpu(&PLAYER_BODY_MESH);
  gpu->copy_static_mesh_to_gpu(&PLAYER_RIGHT_LEG_MESH);
  gpu->copy_static_mesh_to_gpu(&PLAYER_LEFT_LEG_MESH);

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
  EntityId core = ecs__create_entity(ecs);

  ecs__add_player_controller(core, ecs);
  ecs__add_look_at_center(core, ecs);
  ecs__add_velocity(core, (Vec3){0}, ecs);
  ecs__add_radius_collider(core, 0.2f, ecs);
  ecs__add_weapons(
    core,
    (Weapons){
      .primary = fire_lvl0_cannon,
      .primary_autofire_interval = LVL0_CANNON_AUTOFIRE_INTERVAL
    },
    ecs
  );
  ecs__add_transform(
    core,
    (Transform){
      .position = position,
      .scale = 0.8f
    },
    ecs
  );
  ecs__add_draw(
    core,
    (Draw){
      .mesh = &PLAYER_BODY_MESH,
      .textures = CONCRETE_WALL_TEXTURE,
      .shader = &player_shader,
      .draw = ecs__draw_mesh
    },
    ecs
  );

  EntityId left_leg = ecs__create_entity(ecs);

  ecs__add_parent_relationship(core, left_leg, ecs);
  ecs__add_transform(
    left_leg,
    (Transform){
      .position = (Vec3){ -0.4f, -0.1f, 0 },
      .scale = 1,
      .rotation = quaternion__create(WORLDSPACE.up, 0)
    },
    ecs
  );
  ecs__add_draw(
    left_leg,
    (Draw){
      .mesh = &PLAYER_LEFT_LEG_MESH,
      .textures = CONCRETE_WALL_TEXTURE,
      .shader = &player_shader,
      .draw = ecs__draw_mesh
    },
    ecs
  );

  EntityId right_leg = ecs__create_entity(ecs);

  ecs__add_parent_relationship(core, right_leg, ecs);
  ecs__add_transform(
    right_leg,
    (Transform){
      .position = (Vec3){ 0.4f, -0.1f, 0 },
      .scale = 1,
      .rotation = quaternion__create(WORLDSPACE.up, 0)
    },
    ecs
  );
  ecs__add_draw(
    right_leg,
    (Draw){
      .mesh = &PLAYER_RIGHT_LEG_MESH,
      .textures = CONCRETE_WALL_TEXTURE,
      .shader = &player_shader,
      .draw = ecs__draw_mesh
    },
    ecs
  );

  return core;
}

static void draw_firing_guide(
  GameTime time,
  Camera const *const camera,
  GPU const *const gpu,
  Transform const *const hierarchy_transform,
  EntityId id,
  ECS const *const ecs
) {

  gpu->set_shader_float(
    ecs->entities[id].draw.shader,
    "arena_radius_worldspace",
    ARENA_RADIUS
  );
  gpu->set_shader_vec3(
    ecs->entities[id].draw.shader,
    "player_pos_worldspace",
    ecs->entities[PLAYER_ID].transform.position
  );

  ecs__draw_mesh(
    time,
    camera,
    gpu,
    hierarchy_transform,
    id,
    ecs
  );
}

EntityId create_firing_guide(
  ECS *const ecs
) {

  EntityId guide = ecs__create_entity(ecs);

  ecs__add_transform(
    guide,
    (Transform){
      .position = (Vec3){ 0, -0.5f, 0 },
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

void pose_player_mech(
  GameTime time,
  Entity *const player,
  ECS *const ecs
) {

  if (vec3__magnitude(player->velocity) < 0.2f)
    return;

  Vec3 player_velocity_right = vec3__cross(
    vec3__normalize(player->velocity),
    WORLDSPACE.up
  );

  Entity *left_leg =
    &ecs->entities[player->hierarchy.children[PLAYER_LEFT_LEG_INDEX]];
  Entity *right_leg =
    &ecs->entities[player->hierarchy.children[PLAYER_RIGHT_LEG_INDEX]];

  Quaternion rotation = quaternion__create(
    player_velocity_right,
    -M_PI * 0.25f
  );

  left_leg->transform.rotation = rotation;
  right_leg->transform.rotation = rotation;
}