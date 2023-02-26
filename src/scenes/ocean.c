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

static struct Transform COOLING_TOWER_TRANSFORM = {
  .position = { -17, 0.3f, -40 },
  .scale = 5
};

// LOCALS

static struct Gamepad gamepad;
static struct Camera cam;
static EntityId waves;

// MOUNTAINS

// STEAM

static struct Shader steam_shader;

// SKY

// MIST

static struct ECS ecs;
static EntityId mist;
static struct Shader mist_shader;

void ocean__init(
  struct Window const *const window,
  struct Viewport *const vwprt,
  struct GPU const *const gpu
) {

  cam.position = (struct Vec3){ 0, 2, 12 };
  cam.look_target = (struct Vec3){ 0, 2, 0 };
  cam.horizontal_fov_in_deg = 60;
  cam.near_clip_distance = 0.3f;
  cam.far_clip_distance = 100;
  camera__calculate_lookat(WORLDSPACE.up, &cam);
  camera__calculate_perspective(vwprt, &cam);

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
    (struct Transform){
      .scale = 12,
      .rotation = (struct Quaternion){0},
      .position = (struct Vec3){ 1, 4, 2 }
    },
    &ecs
  );
  ecs__add_uv_scroll(
    mist,
    (struct ScrollUV){
      .speed = (struct Vec2){ -0.01f, 0 },
      .total = (struct Vec2){0}
    },
    &ecs
  );
  ecs__add_alpha_effect(mist, &ecs);
  ecs__add_draw(
    mist,
    (struct Draw){
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
    (struct Draw){
      .mesh = &COOLING_TOWER_MESH,
      .textures = CONCRETE_WALL_TEXTURE,
      .shader = &FLAT_TEXTURE_SHADER,
      .draw = ecs__draw_mesh
    },
    &ecs
  );

  // MOUNTAINS

//   mountain_transform = (struct Transform){
//     .position = { 10, -0.4f, -50 },
//     .rotation = quaternion__create(
//       (struct Vec3){ 0, 1, 0 },
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
  struct GameTime time, 
  struct Window const *const window,
  struct Viewport *const vwprt,
  struct GPU const *const gpu,
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

  // struct Vec2 norm_direction =
  //   vec2__normalize(gamepad.left_stick_direction);
  // float magnitude =
  //   vec2__magnitude(gamepad.left_stick_direction);
  // struct Vec2 offset = {0};

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

  ecs__scroll_uvs(time, &ecs);

  ecs__draw(time, &cam, gpu, &ecs);

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