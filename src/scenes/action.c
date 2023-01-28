#include <stdio.h>
#include <math.h>

#include "tail.h"

#include "scene.h"

#include "ecs.h"
#include "ecs_definitions.h"
#include "ecs_systems.h"

#include "constants.h"
#include "bull_math.h"
#include "tail_helpers.h"

#include "rpg.h"
#include "rocket_mesh.h"

#include "player.h"
#include "firing_guide.h"

/*
  ~~~~~~~~~CONSTANTS~~~~~~~~~~
*/

#define PLAYER_LVL_COUNT 3
#define PLAYER_START_POS {3, 0, 0}

#define GUIDE_LAG_TIME_SECONDS 0.15f

#define ARENA_EDGE_RADIUS 8.0f

typedef void (*player_one_autofire_ptr)(
  struct GameTime time,
  struct player const *const playr
);

struct guide_lag_state {
  double seconds_since_player_moved;
  struct vec3 guide_target_position;
};

/*
  ~~~~~~~~~FORWARD DECS~~~~~~~~~~
*/

static void begin_lvl0_rocket_autofire(
  struct GameTime time,
  struct player const *const playr
);

static void autofire_lvl0_rockets(
  struct GameTime time,
  struct player const *const playr
);

static void stop_autofiring(
  struct GameTime time,
  struct player const *const playr
);

void guide_lag_update(
  struct GameTime time,
  struct player const *const playr,
  struct guide_lag_state *const guide_lag
);

/*
  ~~~~~~~~~LOCAL STATE~~~~~~~~~~
*/

static struct ECS ecs;

static struct camera cam;
static struct gamepad_input gamepad;

static struct player player_one = {
  .transform = {
    .position = PLAYER_START_POS,
    .scale = 1
  },
  .previous_position = PLAYER_START_POS,
  .projected_position = PLAYER_START_POS,
  .input_state = PLAYER_INPUT_STATE__IDLE,
  .effect_state = PLAYER_EFFECT_STATE__HEALTHY
};
player_one_autofire_ptr player_one_autofire = NULL;
static double seconds_until_next_autofire_shot;

static struct player_actions player_one_actions ={
  .start_autofire = begin_lvl0_rocket_autofire,
  .stop_autofire = stop_autofiring
};

static struct guide_lag_state guide_lag = {
  .guide_target_position = PLAYER_START_POS
};

static EntityId entities_to_destroy[MAX_ENTITIES];
static uint_fast16_t count_of_entities_to_destroy;
static void mark_entity_for_destruction(EntityId id) {
  entities_to_destroy[count_of_entities_to_destroy++] = id;
}

/*
  ~~~~~~~~~PUBLIC API~~~~~~~~~~
*/

void action__init(
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu
) {

  cam.position = (struct vec3){ 0, 20, 8 };
  cam.look_target = (struct vec3){
    ORIGIN.x,
    ORIGIN.y,
    ORIGIN.z + 1.2f
  };
  cam.horizontal_fov_in_deg = 58;
  cam.near_clip_distance = 0.3f;
  cam.far_clip_distance = 100;

  camera__calculate_lookat(WORLDSPACE.up, &cam);
  camera__calculate_perspective(vwprt, &cam);

  gpu->copy_static_mesh_to_gpu(&rocket_mesh);

  player__copy_assets_to_gpu(gpu);
  firing_guide__copy_assets_to_gpu(gpu);

  ocean__init(window, vwprt, gpu);
}

void action__tick(
  struct GameTime time,
  struct window_api const *const window,
  struct viewport *const vwprt,
  struct gpu_api const *const gpu,
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

  player__update(time, gamepad, &player_one_actions, &player_one);
  guide_lag_update(time, &player_one, &guide_lag);

  if (player_one_autofire) player_one_autofire(
    time,
    &player_one
  );

  // TODO: abstract
  static float player_rads, flip;
  flip = player_one.transform.position.x >= 0 ? (M_PI * 0.5f) : -(M_PI * 0.5f);
  player_rads = atan(
    -player_one.transform.position.z /
    player_one.transform.position.x
  ) + flip;
  player_one.transform._rotation =
    quaternion__create(WORLDSPACE.up, player_rads);

  player_one.transform.position = player_one.projected_position;

  count_of_entities_to_destroy = 0;
  ecs__move(time, &ecs);
  ecs__timeout(time, &ecs);
  for (uint_fast16_t i = 0; i < count_of_entities_to_destroy; i++)
    ecs__destroy_entity(entities_to_destroy[i], &ecs);

  // DRAW

  ocean__tick(time, window, vwprt, gpu, SCENE__MAIN_MENU, NULL);
  gpu->clear_depth_buffer();

  player__draw(&cam, gpu, &player_one);
  firing_guide__draw(
    &cam,
    gpu,
    ARENA_EDGE_RADIUS,
    guide_lag.guide_target_position
  );

  ecs__draw(time, &cam, gpu, &ecs);
}

/*
  ~~~~~~~~ FUNCTION DEFINITIONS ~~~~~~~~~
*/

static void begin_lvl0_rocket_autofire(
  struct GameTime time,
  struct player const *const playr
) {
  player_one_autofire = autofire_lvl0_rockets;
  player_one_autofire(time, playr);
}

static void stop_autofiring(
  struct GameTime time,
  struct player const *const playr
) {
  seconds_until_next_autofire_shot = 0;
  player_one_autofire = NULL;
}

static void autofire_lvl0_rockets(
  struct GameTime time,
  struct player const *const playr
) {

  seconds_until_next_autofire_shot -= time.delta;
  if (seconds_until_next_autofire_shot > 0) return;
  seconds_until_next_autofire_shot = 0.15f;

  struct vec3 velocity = scalar_x_vec3(
    10.0f,
    vec3__normalize(vec3_minus_vec3(
      ORIGIN,
      playr->transform.position
    ))
  );

  EntityId rocket = create_rpg(
    playr->transform.position,
    velocity,
    mark_entity_for_destruction,
    &ecs
  );
}

void guide_lag_update(
  struct GameTime time,
  struct player const *const playr,
  struct guide_lag_state *const guide_lag
) {
  if (vec3__distance(
    playr->previous_position,
    playr->transform.position
  ) <= 0.02) guide_lag->seconds_since_player_moved = 0;
  guide_lag->seconds_since_player_moved += time.delta;
  if (guide_lag->seconds_since_player_moved > GUIDE_LAG_TIME_SECONDS)
    guide_lag->seconds_since_player_moved = GUIDE_LAG_TIME_SECONDS;
  guide_lag->guide_target_position = vec3__lerp(
    guide_lag->guide_target_position,
    playr->transform.position,
    guide_lag->seconds_since_player_moved / GUIDE_LAG_TIME_SECONDS
  );
}