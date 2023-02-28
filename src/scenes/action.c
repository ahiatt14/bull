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
#include "sparks.h"
#include "muzzle_flashes.h"
#include "launchers.h"

#include "drones.h"

#include "rocket_mesh.h"

#include "firing_guide.h"

/*
  ~~~~~~~~~DEFINITIONS~~~~~~~~~~
*/

#define ARENA_EDGE_RADIUS 8.0f

/*
  ~~~~~~~~ENTITY ACTIONS~~~~~~~~
*/

void fire_lvl0_cannon(
  EntityId id,
  Seconds remainder,
  ECS *const ecs
);

/*
  ~~~~~ENTITY EVENT HANDLERS~~~~
*/

void handle_radial_launcher_picked_up_by_player(
  EntityId launcher,
  EntityId player,
  ECS *const ecs
);

void handle_mine_shot_by_player(
  EntityId mine,
  EntityId projectile,
  ECS *const ecs
);

void on_rpg_deployed(
  EntityId rocket,
  Seconds remainder,
  ECS *const ecs
);

void on_rpg_timer_up(
  EntityId rocket,
  Seconds remainder,
  ECS *const ecs
);

void on_player_start_autofire();
void on_player_stop_autofire();

/*
  ~~~~~~~~~LOCAL STATE~~~~~~~~~~
*/

static ECS ecs;

static Camera cam;
static Gamepad gamepad;

static ControllerActions player_one_actions = {
  .on_start_autofire = on_player_start_autofire,
  .on_stop_autofire = on_player_stop_autofire
};

/*
  ~~~~~~~~~PUBLIC API~~~~~~~~~~
*/

void action__init(
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu
) {
  cam.position = (Vec3){ 0, 20, 14 };
  cam.look_target = (Vec3){
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

  explosions__copy_assets_to_gpu(gpu);
  muzzle_flashes__copy_assets_to_gpu(gpu);

  player__copy_assets_to_gpu(gpu);
  create_player(
    (Vec3){ 3, 0, 0 },
    fire_lvl0_cannon,
    &ecs
  );
  firing_guide__copy_assets_to_gpu(gpu);

  ocean__init(window, vwprt, gpu);

  // start_drone_pattern_0(&ecs);

  launchers__copy_assets_to_gpu(gpu);
  launchers__init_scene_callbacks(
    handle_radial_launcher_picked_up_by_player,
    window->get_seconds_since_creation
  );
  create_radial_launcher_spawner(&ecs);
}

void action__tick(
  GameTime time,
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu,
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

  ecs__control_player(
    time,
    gamepad,
    &player_one_actions,
    &ecs.entities[PLAYER_ID]
  );
  ecs__timeout(time, &ecs);
  ecs__repeat(time, &ecs);
  ecs__lerp_vec3(time, &ecs);
  ecs__lerp_revolve(time, &ecs);
  ecs__gravity(time, &ecs);
  ecs__move(time, &ecs);
  ecs__look_at_center(time, &ecs);
  ecs__scroll_uvs(time, &ecs);
  ecs__check_projectile_radius_collisions(time, &ecs);
  ecs__check_pickup_radius_collisions(time, &ecs);

  ecs__destroy_marked_entities(&ecs);

  // DRAW

  ocean__tick(time, window, vwprt, gpu, SCENE__MAIN_MENU, NULL);
  gpu->clear_depth_buffer();

  gpu->cull_back_faces();

  ecs__draw(time, &cam, gpu, &ecs);

  firing_guide__draw(
    &cam,
    gpu,
    ARENA_EDGE_RADIUS,
    ecs.entities[PLAYER_ID].transform.position
  );
}

// void guide_lag_update(
//   GameTime time,
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
    PLAYER_ID,
    (Repeat){
      .age = 0,
      .interval = ecs.entities[PLAYER_ID].weapons.primary_autofire_interval,
      .on_interval = ecs.entities[PLAYER_ID].weapons.primary
    },
    &ecs
  );
}

void on_player_stop_autofire() {
  ecs__remove_repeat(PLAYER_ID, &ecs);
}

void fire_lvl0_cannon(
  EntityId weapon,
  Seconds remainder,
  ECS *const ecs
) {
  
  // TOOD: add muzzle flash

  Vec3 direction =
    vec3__normalize(vec3_minus_vec3(
      ORIGIN,
      ecs->entities[weapon].transform.position
    ));

  create_lvl0_cannonfire(
    ecs->entities[weapon].transform.position,
    direction,
    remainder,
    ecs
  );

  // create_lvl0_muzzle_flash(
  //   &ecs->entities[weapon].transform,
  //   ecs
  // );
}

void on_rpg_deployed(
  EntityId rocket,
  Seconds remainder,
  ECS *const ecs
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
  ECS *const ecs
) {

  ecs__mark_for_destruction(rocket, ecs);
  create_rpg_explosion(
    rocket,
    cam.position, // TODO: turn cam into an ECS obj with known
    // id (prolly 1) so systems can access it as needed
    ecs
  );

  // TODO: damage radius
}

static void return_player_control(
  EntityId player,
  Seconds remainder,
  ECS *const ecs
) {
  ecs__add_player_controller(player, ecs);
  ecs__add_look_at_center(player, ecs);
  ecs->entities[player].draw.shader = &player_shader;
  ecs__remove_vec3lerp(player, ecs);
}
void handle_radial_launcher_picked_up_by_player(
  EntityId launcher,
  EntityId player,
  ECS *const ecs
) {

  static float LAUNCH_SPEED = 40;

  ecs__mark_for_destruction(launcher, ecs);

  Vec3 start_position = ecs->entities[launcher].transform.position;
  Vec3 end_position = space__ccw_angle_rotate(
    WORLDSPACE.up,
    M_PI,
    start_position
  );

  float travel_time =
    vec3__distance(start_position, end_position) /
    LAUNCH_SPEED;

  ecs__remove_player_controller(player, ecs);
  ecs__remove_look_at_center(player, ecs);
  ecs->entities[player].draw.shader = &SOLID_COLOR_SHADER;
  ecs__add_vec3lerp(
    player,
    (Vec3Lerp){
      .start = start_position,
      .end = end_position,
      .age = 0,
      .duration = travel_time,
      .lerp = vec3__linear_lerp,
      .on_finish = return_player_control
    },
    ecs
  );

  create_sparks(
    start_position,
    scalar_x_vec3(
      LAUNCH_SPEED * 0.25f,
      vec3_minus_vec3(start_position, ORIGIN)
    ),
    25,
    ecs
  );
}

void handle_mine_shot_by_player(
  EntityId mine,
  EntityId projectile,
  ECS *const ecs
) {
  // create_rpg_explosion(
  //   projectile,
  //   cam.position,
  //   ecs
  // );
  create_sparks(
    ecs->entities[projectile].transform.position,
    ecs->entities[projectile].velocity,
    4,
    ecs
  );
  // ecs__mark_for_destruction(mine, ecs);
  ecs__mark_for_destruction(projectile, ecs);
}