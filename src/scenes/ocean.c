#include <stdlib.h>
#include <stdint.h>
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
#include "cloud_cover.h"
#include "cloud_wall.h"

#include "mountain_mesh.h"
#include "standard_material_frag.h"
#include "default_vert.h"

// LIGHTS

void create_light_row();

// LOCALS

static ECS ecs;

static Camera camera;
static Vec3 camera_look_target = { 200, 150, 0 };

static Lighting lighting = {
  .point_count = 0,
  .ambient = {
    .color = COLOR_WARM_SUNSET,
    .strength = 0.28f
  },
  .sky = {
    .color = COLOR_MOONLIGHT,
    .strength = 0.5f
  }
};

static Shader solid_material_shader;

void ocean__init(
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu
) {

  camera.position = (Vec3){ 50, 10, 700 };
  camera.look_target = camera_look_target;
  camera.horizontal_fov_in_deg = 80;
  camera.near_clip_distance = 0.4f;
  camera.far_clip_distance = 3000;
  camera__calculate_lookat(WORLDSPACE.up, &camera);
  camera__calculate_perspective(vwprt, &camera);

  lighting.sky.direction = vec3__normalize((Vec3){ -1, 0, 0 });

  ocean_skybox__copy_assets_to_gpu(gpu);
  
  cloud_cover__copy_assets_to_gpu(gpu);
  create_cloud_cover(&ecs);
  cloud_wall__copy_assets_to_gpu(gpu);
  create_cloud_wall(&ecs);

  solid_material_shader.frag_src = STANDARD_MATERIAL_FRAG_SRC;
  solid_material_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&solid_material_shader);

  create_light_row();

  // MOUNTAIN

  gpu->copy_static_mesh_to_gpu(&MOUNTAIN_MESH);
  EntityId mountain = ecs__create_entity(&ecs);
  ecs__add_transform(
    mountain,
    (Transform){
      .position = (Vec3){ 800, 0, -1200 },
      .scale = 400,
      .rotation = quaternion__create(WORLDSPACE.up, M_PI * -0.35f)
    },
    &ecs
  );
  ecs__add_receives_light(mountain, &ecs);
  ecs__add_draw(
    mountain,
    (Draw){
      .mesh = &MOUNTAIN_MESH,
      .draw = ecs__draw_mesh,
      .textures = MOUNTAIN_TEXTURE,
      .shader = &solid_material_shader
    },
    &ecs
  ); 

  water__copy_assets_to_gpu(gpu);
  create_water(&ecs);

  plume_plant__copy_assets_to_gpu(gpu);
  create_plume_plant((Vec3){0}, &ecs);
  create_plume_plant((Vec3){ 400, 0, -500 }, &ecs);
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
  draw_ocean_skybox(time, &camera, gpu);
  gpu->clear_depth_buffer();

  ecs__draw(time, &camera, &lighting, gpu, &ecs);
}

void turn_light_on(EntityId light) {
 ecs__add_point_light_source(
    light,
    (PointLight){
      .color = COLOR_RED,
      .strength = 400
    },
    &ecs
  );
  ecs__add_draw(
    light,
    (Draw){
      .mesh = NULL,
      .textures = COOLING_TOWER_LIGHT_TEXTURE,
      .shader = &DEFAULT_BILLBOARD_SHADER,
      .draw = ecs__draw_billboard
    },
    &ecs
  );
}
void toggle_light(
  EntityId light,
  Seconds remainder,
  ECS *const ecs
) {
  if (lacks_components(
    c_POINT_LIGHT | c_DRAW,
    ecs->entities[light].config
  )) {
    turn_light_on(light);
  } else {
    ecs__remove_point_light_source(light, ecs);
    ecs__remove_draw(light, ecs);
  }
}

void create_light_row() {

  static const uint_fast8_t LIGHT_COUNT = 7;

  Vec3 start = (Vec3){ 400, 40, -400 };
  Vec3 offset = (Vec3){ -80, 0, 140 };

  for (int i = 0; i < LIGHT_COUNT; i++) {
    EntityId light = ecs__create_entity(&ecs);
    ecs__add_transform(
      light,
      (Transform){
        .position = vec3_plus_vec3(start, scalar_x_vec3(i, offset)),
        .scale = 5,
        .rotation = quaternion__create(WORLDSPACE.up, 0)
      },
      &ecs
    );
    ecs__add_alpha_effect(light, &ecs);
    if (i % 2 == 0) turn_light_on(light);
    ecs__add_repeat(
      light,
      (Repeat){
        .age = 0,
        .interval = 1,
        .on_interval = toggle_light
      },
      &ecs
    );
  }
}