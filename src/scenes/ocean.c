#include <stdlib.h>
#include <math.h>

#include "tail.h"

#include "ecs.h"

#include "lcd_text.h"

#include "scene.h"

#include "assets.h"
#include "constants.h"
#include "bull_math.h"
#include "tail_helpers.h"

#include "ocean_skybox.h"
#include "water.h"
#include "plume_plant.h"

// LOCALS

static ECS ecs;

static Camera camera;
 
static Vec3 camera_look_target = {
  200,
  250,
  0
};

static EntityId plume_plant;
static EntityId waves;

static NumberReadout entity_count_debug = {
  .position = (Vec2){ -0.9f, 0.9f },
  .scale = 0.05f
};

void ocean__init(
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu
) {

  camera.position = (Vec3){ -600, 8, 800 };
  camera.look_target = camera_look_target;
  camera.horizontal_fov_in_deg = 80;
  camera.near_clip_distance = 1;
  camera.far_clip_distance = 8000;
  camera__calculate_lookat(WORLDSPACE.up, &camera);
  camera__calculate_perspective(vwprt, &camera);

  // ocean_skybox__copy_assets_to_gpu(gpu);

  water__copy_assets_to_gpu(gpu);
  waves = create_water(&ecs);

  plume_plant__copy_assets_to_gpu(gpu);
  plume_plant = create_plume_plant((Vec3){0}, &ecs);

  EntityId light;
  Vec3 light_position = (Vec3){ 0, 140, -500 };
  for (int i = 0; i < 8; i++) {
    
    light = ecs__create_entity(&ecs);
    ecs__add_transform(
      light,
      (Transform){
        .position = space__ccw_angle_rotate(
          WORLDSPACE.up,
          ((float)i / 8.0f) * (M_PI * 2.0f),
          light_position
        ),
        .scale = 10
      },
      &ecs
    );
    ecs__add_alpha_effect(light, &ecs);
    ecs__add_draw(
      light,
      (Draw){
        .draw = ecs__draw_billboard,
        .textures = COOLING_TOWER_LIGHT_TEXTURE,
        .shader = &DEFAULT_BILLBOARD_SHADER,
        .mesh = NULL
      },
      &ecs
    );
  }
}

void ocean__tick(
  GameTime time, 
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu,
  uint8_t previous_scene,
  void (*switch_scene)(uint8_t new_scene)
) {

  // UPDATE

  // camera.look_target = space__ccw_angle_rotate(
  //   WORLDSPACE.up,
  //   CAM_REVOLVE_SPEED * time.delta,
  //   camera.look_target
  // );
  // camera.position = space__ccw_angle_rotate(
  //   WORLDSPACE.up,
  //   CAM_REVOLVE_SPEED * time.delta,
  //   camera.position
  // );
  // camera.position.z -= 20.0f * time.delta;
  camera__calculate_lookat(WORLDSPACE.up, &camera);
  camera__calculate_perspective(vwprt, &camera);

  ecs__scroll_uvs(time, &ecs);
  ecs__repeat(time, &ecs);
  ecs__timeout(time, &ecs);
  ecs__lerp_rotation(time, &ecs);
  ecs__move(time, &ecs);
  ecs__destroy_marked_entities(&ecs);

  // DRAW

  // TODO: can optimize here per learnopengl cubemap article
  // draw_ocean_skybox(&camera, gpu);
  gpu->clear_depth_buffer();

  ecs__draw(time, &camera, gpu, &ecs);

  entity_count_debug.value = ecs.count;
  lcd_text__draw_number(entity_count_debug, gpu);
}