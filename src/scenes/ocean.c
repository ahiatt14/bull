#include <stdlib.h>
#include <math.h>

#include "tail.h"

#include "ecs.h"

#include "lcd_text.h"

#include "scene.h"

#include "colors.h"
#include "lighting.h"
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
static Vec3 camera_look_target = { 200, 250, 0 };

static Lighting lighting = {
  .point_count = 0,
  .ambient = {
    .color = COLOR_WHITE,
    .strength = 0.05f
  },
  .sky = {
    .direction = { -1, 0, 0 },
    .color = COLOR_EVENING_SUNLIGHT,
    .strength = 0.2f
  }
};

void ocean__init(
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu
) {

  camera.position = (Vec3){ -200, 8, 500 };
  camera.look_target = camera_look_target;
  camera.horizontal_fov_in_deg = 80;
  camera.near_clip_distance = 1;
  camera.far_clip_distance = 2000;
  camera__calculate_lookat(WORLDSPACE.up, &camera);
  camera__calculate_perspective(vwprt, &camera);

  ocean_skybox__copy_assets_to_gpu(gpu);

  water__copy_assets_to_gpu(gpu);
  create_water(&ecs);

  plume_plant__copy_assets_to_gpu(gpu);
  create_plume_plant((Vec3){0}, &ecs);
}

void ocean__tick(
  GameTime time, 
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu,
  uint8_t previous_scene,
  void (*switch_scene)(uint8_t new_scene)
) {

  ecs__scroll_uvs(time, &ecs);
  ecs__repeat(time, &ecs);
  ecs__timeout(time, &ecs);
  ecs__lerp_rotation(time, &ecs);
  ecs__move(time, &ecs);
  ecs__destroy_marked_entities(&ecs);

  // DRAW

  // TODO: can optimize here per learnopengl cubemap article
  draw_ocean_skybox(&camera, gpu);
  gpu->clear_depth_buffer();

  ecs__draw(time, &camera, &lighting, gpu, &ecs);
}