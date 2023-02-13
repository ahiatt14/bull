#include <stdio.h>
#include <math.h>

#include "tail.h"

#include "scene.h"

#include "ecs.h"

#include "constants.h"
#include "bull_math.h"
#include "tail_helpers.h"

#include "player.h"
#include "guns.h"
#include "rpg.h"
#include "explosions.h"
#include "muzzle_flashes.h"

#include "mines.h"

#include "rocket_mesh.h"

#include "firing_guide.h"

/*
  ~~~~~~~~~DEFINITIONS~~~~~~~~~~
*/

#define ENTITY_CALLBACK_PARAMS EntityId id, Seconds remainder, struct ECS *const ecs

#define PLAYER_LVL_COUNT 3

#define GUIDE_LAG_TIME_SECONDS 0.15f

#define ARENA_EDGE_RADIUS 8.0f

static const struct Vec3 PLAYER_START_POSITION = { 3, 0, 0 };

/*
  ~~~~~~~~ENTITY ACTIONS~~~~~~~~
*/

void fire_lvl0_cannon(
  EntityId id,
  Seconds remainder,
  struct ECS *const ecs
);

/*
  ~~~~~ENTITY EVENT HANDLERS~~~~
*/

void on_rpg_deployed(
  EntityId rocket,
  Seconds remainder,
  struct ECS *const ecs
);

void on_rpg_timer_up(
  EntityId rocket,
  Seconds remainder,
  struct ECS *const ecs
);

void on_player_start_autofire();
void on_player_stop_autofire();

/*
  ~~~~~~~~~LOCAL STATE~~~~~~~~~~
*/

static struct ECS ecs;

static struct Camera cam;
static struct Gamepad gamepad;

static EntityId player;

static struct ControllerActions player_one_actions = {
  .on_start_autofire = on_player_start_autofire,
  .on_stop_autofire = on_player_stop_autofire
};

static EntityId entities_to_destroy[MAX_ENTITIES];
static uint_fast16_t count_of_entities_to_destroy;
static void mark_entity_for_destruction(
  EntityId id,
  Seconds remainder,
  struct ECS *const ecs
) {
  entities_to_destroy[count_of_entities_to_destroy++] = id;
}

/*
  ~~~~~~~~~PUBLIC API~~~~~~~~~~
*/

void action__init(
  struct Window const *const window,
  struct Viewport *const vwprt,
  struct GPU const *const gpu
) {

  cam.position = (struct Vec3){ 0, 20, 14 };
  cam.look_target = (struct Vec3){
    ORIGIN.x,
    ORIGIN.y,
    ORIGIN.z + 1.2f
  };
  cam.horizontal_fov_in_deg = 58;
  cam.near_clip_distance = 0.3f;
  cam.far_clip_distance = 100;

  camera__calculate_lookat(WORLDSPACE.up, &cam);
  camera__calculate_perspective(vwprt, &cam);

  gpu->copy_static_mesh_to_gpu(&ROCKET_MESH); // TODO: move into rpg file

  guns__copy_assets_to_gpu(gpu);
  explosions__copy_assets_to_gpu(gpu);
  muzzle_flashes__copy_assets_to_gpu(gpu);

  player__copy_assets_to_gpu(gpu);
  player = create_player(
    PLAYER_START_POSITION,
    fire_lvl0_cannon,
    &ecs
  );
  firing_guide__copy_assets_to_gpu(gpu);

  ocean__init(window, vwprt, gpu);

  mines__copy_assets_to_gpu(gpu);
  create__mine(
    (struct Vec3){ -4, 0, -4 },
    (struct Vec3){ -4, 0, 0 },
    1,
    &ecs
  );
}

void action__tick(
  struct GameTime time,
  struct Window const *const window,
  struct Viewport *const vwprt,
  struct GPU const *const gpu,
  uint8_t previous_scene,
  void switch_scene(uint8_t new_scene)
) {

  window->get_gamepad_input(&gamepad);

  // HANDLE UNPLUGGED GAMEPAD
  
  // if (!window->gamepad_is_connected()) {
  //   switch_scene(SCENE__CONNECT_GAMEPAD);
  //   return;
  // }

  // GAMEPLAY

  count_of_entities_to_destroy = 0;
  ecs__control_player(
    time,
    gamepad,
    &player_one_actions,
    &ecs.entities[player]
  );
  ecs__timeout(time, &ecs);
  ecs__repeat(time, &ecs);
  ecs__lerp_vec3(time, &ecs);
  ecs__lerp_revolve(time, &ecs);
  ecs__gravity(time, &ecs);
  ecs__move(time, &ecs);
  ecs__look_at_center(time, &ecs);
  ecs__scroll_uvs(time, &ecs);
  for (uint_fast16_t i = 0; i < count_of_entities_to_destroy; i++)
    ecs__destroy_entity(entities_to_destroy[i], &ecs);

  // DRAW

  ocean__tick(time, window, vwprt, gpu, SCENE__MAIN_MENU, NULL);
  gpu->clear_depth_buffer();

  ecs__draw(time, &cam, gpu, &ecs);

  firing_guide__draw(
  &cam,
  gpu,
  ARENA_EDGE_RADIUS,
  ecs.entities[player].transform.position
);
}

/*
  ~~~~~~~~ FUNCTION DEFINITIONS ~~~~~~~~~
*/

// static void begin_lvl0_gun_autofire(
//   EntityId player
// ) {
//   player_one_autofire = autofire_lvl0_guns;
//   player_one_autofire(time, player);
// }

// static void autofire_lvl0_guns(
//   struct GameTime time,
//   struct Entity const *const player
// ) {

//   seconds_until_next_autofire_shot -= time.delta;
//   if (seconds_until_next_autofire_shot > 0) return;
//   seconds_until_next_autofire_shot =
//     0.15f - seconds_until_next_autofire_shot;
    
//   create_lvl0_muzzle_flash(
//     &player->transform,
//     mark_entity_for_destruction,
//     &ecs
//   );
// }

// static void begin_lvl0_rocket_autofire(
//   struct GameTime time,
//   struct Player const *const player
// ) {
//   player_one_autofire = autofire_lvl0_rockets;
//   player_one_autofire(time, player);
// }

// static void autofire_lvl0_rockets(
//   struct GameTime time,
//   struct Player const *const player
// ) {

//   seconds_until_next_autofire_shot -= time.delta;
//   if (seconds_until_next_autofire_shot > 0) return;
//   seconds_until_next_autofire_shot =
//     0.15f - seconds_until_next_autofire_shot;

//   deploy_rpg(
//     player->transform.position,
//     on_rpg_deployed,
//     &ecs
//   );
// }

// static void stop_autofiring(
//   struct GameTime time,
//   struct Player const *const player
// ) {
//   seconds_until_next_autofire_shot = 0;
//   player_one_autofire = NULL;
// }

// void guide_lag_update(
//   struct GameTime time,
//   struct Player const *const player,
//   struct guide_lag_state *const guide_lag
// ) {
//   if (vec3__distance(
//     player->previous_position,
//     player->transform.position
//   ) <= 0.02) guide_lag->seconds_since_player_moved = 0;
//   guide_lag->seconds_since_player_moved += time.delta;
//   if (guide_lag->seconds_since_player_moved > GUIDE_LAG_TIME_SECONDS)
//     guide_lag->seconds_since_player_moved = GUIDE_LAG_TIME_SECONDS;
//   guide_lag->guide_target_position = vec3__linear_lerp(
//     guide_lag->guide_target_position,
//     player->transform.position,
//     guide_lag->seconds_since_player_moved / GUIDE_LAG_TIME_SECONDS
//   );
// }

/*
  ~~~~~ENTITY EVENT HANDLERS~~~~
*/

void on_player_start_autofire() {
  ecs__add_repeat(
    player,
    (struct Repeat){
      .age = 0,
      .interval = ecs.entities[player].weapons.primary_autofire_interval,
      .on_interval = ecs.entities[player].weapons.primary
    },
    &ecs
  );
}

void on_player_stop_autofire() {
  ecs__remove_repeat(player, &ecs);
}

void fire_lvl0_cannon(
  EntityId weapon,
  Seconds remainder,
  struct ECS *const ecs
) {
  
  // TOOD: add muzzle flash

  struct Vec3 direction =
    vec3__normalize(vec3_minus_vec3(
      ORIGIN,
      ecs->entities[weapon].transform.position
    ));

  struct Vec3 target = vec3_plus_vec3(
    scalar_x_vec3(30, direction),
    ecs->entities[weapon].transform.position
  );

  struct Vec3 starting_position =
    vec3__linear_lerp(
      ecs->entities[weapon].transform.position,
      target,
      remainder
    );

  // TODO: add remainder to starting position
  create_lvl0_cannonfire(
    starting_position,
    direction,
    remainder,
    mark_entity_for_destruction,
    ecs
  );

  // create_lvl0_muzzle_flash(
  //   &ecs->entities[weapon].transform,
  //   mark_entity_for_destruction,
  //   ecs
  // );
}

void on_rpg_deployed(
  EntityId rocket,
  Seconds remainder,
  struct ECS *const ecs
) {

  propel_rpg(
    rocket,
    remainder,
    on_rpg_timer_up,
    ecs
  );

  // create_rpg_thruster_blink(rocket, ecs);

  // TODO: spawn thruster effect (can do inside propel_rpg)
}

// TODO: using this standardized callback signature,
// we can move lotsa these ancillary callbacks
// into their parent files
void on_rpg_timer_up(
  EntityId rocket,
  Seconds remainder,
  struct ECS *const ecs
) {

  mark_entity_for_destruction(rocket, remainder, ecs);
  create_rpg_explosion(
    rocket,
    cam.position,
    mark_entity_for_destruction,
    ecs
  );

  // TODO: damage radius
}