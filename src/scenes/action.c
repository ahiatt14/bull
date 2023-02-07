#include <stdio.h>
#include <math.h>

#include "tail.h"

#include "scene.h"

#include "ecs.h"

#include "constants.h"
#include "bull_math.h"
#include "tail_helpers.h"

#include "rpg.h"
#include "explosions.h"
#include "muzzle_flashes.h"

#include "rocket_mesh.h"

#include "player.h"
#include "firing_guide.h"

/*
  ~~~~~~~~~CONSTANTS~~~~~~~~~~
*/

#define PLAYER_LVL_COUNT 3

#define GUIDE_LAG_TIME_SECONDS 0.15f

#define ARENA_EDGE_RADIUS 8.0f

static const struct Vec3 PLAYER_START_POSITION = { 3, 0, 0 };

/*
  ~~~~~ENTITY EVENT HANDLERS~~~~
*/

void on_rpg_deployed(
  EntityId id,
  double remainder_in_seconds,
  struct ECS *const ecs
);

void on_rpg_timer_up(
  EntityId id,
  double remainder_in_seconds,
  struct ECS *const ecs
);

/*
  ~~~~~~~~~LOCAL STATE~~~~~~~~~~
*/

static struct ECS ecs;

static struct Camera cam;
static struct Gamepad gamepad;

static EntityId player;

// static struct PlayerActions player_one_actions = {
//   .start_autofire = begin_lvl0_gun_autofire,
//   .stop_autofire = stop_autofiring
// };

static EntityId entities_to_destroy[MAX_ENTITIES];
static uint_fast16_t count_of_entities_to_destroy;
static void mark_entity_for_destruction(
  EntityId id,
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

  gpu->copy_static_mesh_to_gpu(&ROCKET_MESH);

  explosions__copy_assets_to_gpu(gpu);
  muzzle_flashes__copy_assets_to_gpu(gpu);

  player__copy_assets_to_gpu(gpu);
  player = create_player(PLAYER_START_POSITION, &ecs);
  firing_guide__copy_assets_to_gpu(gpu);

  ocean__init(window, vwprt, gpu);
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
  ecs__control_player(time, gamepad, &ecs.entities[player]);
  ecs__timeout(time, &ecs);
  ecs__lerp_vec3(time, &ecs);
  ecs__lerp_revolve(time, &ecs);
  ecs__move(time, &ecs);
  ecs__look_at_center(time, &ecs);
  for (uint_fast16_t i = 0; i < count_of_entities_to_destroy; i++)
    ecs__destroy_entity(entities_to_destroy[i], &ecs);

  // DRAW

  ocean__tick(time, window, vwprt, gpu, SCENE__MAIN_MENU, NULL);
  gpu->clear_depth_buffer();

  ecs__draw(time, &cam, gpu, &ecs);
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

void on_rpg_deployed(
  EntityId rocket,
  double remainder_in_seconds,
  struct ECS *const ecs
) {

  propel_rpg(
    rocket,
    remainder_in_seconds,
    on_rpg_timer_up,
    ecs
  );

  // create_rpg_thruster_blink(rocket, ecs);

  // TODO: spawn thruster effect (can do inside propel_rpg)
}

void on_rpg_timer_up(
  EntityId rocket,
  double remainder_in_seconds,
  struct ECS *const ecs
) {

  mark_entity_for_destruction(rocket, ecs);
  create_rpg_explosion(
    ecs->entities[rocket].transform.position,
    mark_entity_for_destruction,
    ecs
  );

  // TODO: damage radius
}