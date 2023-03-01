#include <stdlib.h>
#include <math.h>

#include "tail.h"

#include "ecs.h"

#include "scene.h"

#include "assets.h"
#include "constants.h"
#include "bull_math.h"
#include "tail_helpers.h"

#include "cooling_tower_mesh.h"

#include "ocean_skybox.h"
#include "water.h"

#include "mountain_mesh.h"
#include "mountain_frag.h"
#include "default_vert.h"

#include "flat_texture_frag.h"
#include "steam_column_mesh.h"
#include "steam_frag.h"
#include "steam_geo.h"

#include "billboard_geo.h"
#include "billboard_vert.h"

#include "sky_mesh.h"
#include "mist_frag.h"

// READ ONLY

static Transform COOLING_TOWER_TRANSFORM = {
  .position = {0},
  .scale = 20
};

static float CAM_REVOLVE_SPEED = (M_PI / 200.0f);

// LOCALS

static ECS ecs;

// static Gamepad gamepad;
static Camera camera;
 
static Vec3 camera_look_target = {
  40,
  2,
  0
};

// WATER
static EntityId waves;

// MOUNTAINS

// STEAM
// static Shader steam_shader;

// MIST
static EntityId mist;
static Shader mist_shader;

void ocean__init(
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu
) {

  camera.position = (Vec3){ 25, 2, 150 };
  camera.look_target = camera_look_target;
  camera.horizontal_fov_in_deg = 80;
  camera.near_clip_distance = 0.3f;
  camera.far_clip_distance = 200;
  camera__calculate_lookat(WORLDSPACE.up, &camera);
  camera__calculate_perspective(vwprt, &camera);

  // STEAM

  // steam_shader.frag_src = FLAT_TEXTURE_FRAG_SRC;
  // steam_shader.geo_src = STEAM_GEO_SRC;
  // steam_shader.vert_src = DEFAULT_VERT_SRC;
  // gpu->copy_shader_to_gpu(&steam_shader);
  // gpu->copy_dynamic_mesh_to_gpu(&STEAM_COLUMN_MESH);
  // space__create_model(
  //   &WORLDSPACE,
  //   &COOLING_TOWER_TRANSFORM,
  //   &steam_local_to_world
  // );
  // space__create_normals_model(
  //   &steam_local_to_world,
  //   &steam_normals_local_to_world
  // );

  // SKY

  ocean_skybox__copy_assets_to_gpu(gpu);

  // OCEAN
  water__copy_assets_to_gpu(gpu);
  waves = create_water(&ecs);

  // MIST
  mist_shader.frag_src = MIST_FRAG_SRC;
  mist_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&mist_shader);
  gpu->copy_static_mesh_to_gpu(&SKY_MESH);
  mist = ecs__create_entity(&ecs);
  ecs__add_transform(
    mist,
    (Transform){
      .scale = 100,
      .rotation = (Quaternion){0},
      .position = (Vec3){0}
    },
    &ecs
  );
  ecs__add_uv_scroll(
    mist,
    (ScrollUV){
      .speed = (Vec2){ -0.01f, 0 },
      .total = (Vec2){0}
    },
    &ecs
  );
  ecs__add_alpha_effect(mist, &ecs);
  ecs__add_draw(
    mist,
    (Draw){
      .shader = &mist_shader,
      .textures = MIST_TEXTURE,
      .mesh = &SKY_MESH,
      .draw = ecs__draw_mesh
    },
    &ecs
  );

  // COOLING TOWER

  EntityId cooling_tower = ecs__create_entity(&ecs);

  gpu->copy_static_mesh_to_gpu(&COOLING_TOWER_MESH);
  ecs__add_transform(
    cooling_tower,
    COOLING_TOWER_TRANSFORM,
    &ecs
  );
  ecs__add_draw(
    cooling_tower,
    (Draw){
      .mesh = &COOLING_TOWER_MESH,
      .textures = CONCRETE_WALL_TEXTURE,
      .shader = &FLAT_TEXTURE_SHADER,
      .draw = ecs__draw_mesh
    },
    &ecs
  );

  // MOUNTAINS

//   mountain_transform = (Transform){
//     .position = { 10, -0.4f, -50 },
//     .rotation = quaternion__create(
//       (Vec3){ 0, 1, 0 },
//       -M_PI * 0.25f
//     ),
//     .scale = 13
//   };
//   mountain_shader.frag_src = MOUNTAIN_FRAG_SRC;
//   mountain_shader.vert_src = DEFAULT_VERT_SRC;
//   gpu->copy_shader_to_gpu(&mountain_shader);
//   mesh__tile_uvs(2, 2, &MOUNTAIN_MESH);
//   gpu->copy_static_mesh_to_gpu(&MOUNTAIN_MESH);
//   space__create_model(
//     &WORLDSPACE,
//     &mountain_transform,
//     &mountain_local_to_world
//   );
//   space__create_normals_model(
//     &mountain_local_to_world,
//     &mountain_normals_local_to_world
//   );
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

  // for (unsigned int i = 0; i < STEAM_COLUMN_MESH.vertices_length; i++) {
  //   STEAM_COLUMN_MESH.vertices[i].uv.x += 0.06f * time.delta;
  // }
  // gpu->update_gpu_mesh_data(&STEAM_COLUMN_MESH);

  // TODO: DEBUGGING
  // window->get_gamepad_input(&gamepad);

  // static const float SPEED = 0.2f;
  // static const float STICK_DEADZONE = 0.2f;

  // Vec2 norm_direction =
  //   vec2__normalize(gamepad.left_stick_direction);
  // float magnitude =
  //   vec2__magnitude(gamepad.left_stick_direction);
  // Vec2 offset = {0};

  // if (magnitude >= STICK_DEADZONE) {
  //   offset.x =
  //     SPEED * (magnitude + 1.0f) * magnitude *
  //     norm_direction.x;
  //   offset.y =
  //     SPEED * (magnitude + 1.0f) * magnitude *
  //     norm_direction.y;

  //   cam.position.x += offset.x;
  //   cam.position.z += offset.y;

  //   cam.look_target.x += offset.x;
  //   cam.look_target.z += offset.y;
  
  //   camera__calculate_lookat(WORLDSPACE.up, &cam);
  //   camera__calculate_perspective(vwprt, &cam);
  // }

  camera.look_target = space__ccw_angle_rotate(
    WORLDSPACE.up,
    CAM_REVOLVE_SPEED * time.delta,
    camera.look_target
  );
  camera.position = space__ccw_angle_rotate(
    WORLDSPACE.up,
    CAM_REVOLVE_SPEED * time.delta,
    camera.position
  );
  camera__calculate_lookat(WORLDSPACE.up, &camera);
  camera__calculate_perspective(vwprt, &camera);

  ecs__scroll_uvs(time, &ecs);

  // DRAW

  draw_ocean_skybox(&camera, gpu);
  gpu->clear_depth_buffer();

  ecs__draw(time, &camera, gpu, &ecs);

  // STEAM

  // gpu->select_shader(&steam_shader);
  // // gpu->select_texture(&STEAM_TAIL_TEXTURE);
  // gpu->set_shader_float(
  //   &steam_shader,
  //   "speed",
  //   -0.36f
  // );
  // gpu->set_shader_float(
  //   &steam_shader,
  //   "seconds_since_activation",
  //   time.sec_since_game_launch
  // );
  // gpu->set_shader_float(
  //   &steam_shader,
  //   "max_altitude",
  //   2
  // );
  // gpu__set_mvp(
  //   &steam_local_to_world,
  //   &steam_normals_local_to_world,
  //   &cam,
  //   &steam_shader,
  //   gpu
  // );
  // gpu->draw_mesh(&STEAM_COLUMN_MESH);
}