#include <stdlib.h>
#include <math.h>

#include "tail.h"

#include "ecs.h"

#include "scene.h"

#include "assets.h"
#include "constants.h"
#include "bull_math.h"
#include "tail_helpers.h"

#include "ocean_skybox.h"
#include "water.h"
#include "plume_plant.h"

#include "mist_mesh.h"
#include "mist_frag.h"
#include "default_vert.h"

// READ ONLY

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

static EntityId plume_plant;
static EntityId waves;
static EntityId mist;
static Shader mist_shader;

void ocean__init(
  Window const *const window,
  Viewport *const vwprt,
  GPU const *const gpu
) {

  camera.position = (Vec3){ 25, 2, 120 };
  camera.look_target = camera_look_target;
  camera.horizontal_fov_in_deg = 80;
  camera.near_clip_distance = 0.3f;
  camera.far_clip_distance = 300;
  camera__calculate_lookat(WORLDSPACE.up, &camera);
  camera__calculate_perspective(vwprt, &camera);

  ocean_skybox__copy_assets_to_gpu(gpu);

  water__copy_assets_to_gpu(gpu);
  waves = create_water(&ecs);

  plume_plant__copy_assets_to_gpu(gpu);
  plume_plant = create_plume_plant((Vec3){0}, &ecs);

  // MIST
  mist_shader.frag_src = MIST_FRAG_SRC;
  mist_shader.vert_src = DEFAULT_VERT_SRC;
  gpu->copy_shader_to_gpu(&mist_shader);
  gpu->copy_static_mesh_to_gpu(&MIST_MESH);
  mist = ecs__create_entity(&ecs);
  ecs__add_transform(
    mist,
    (Transform){
      .scale = 150,
      .rotation = (Quaternion){0},
      .position = (Vec3){0}
    },
    &ecs
  );
  ecs__add_uv_scroll(
    mist,
    (ScrollUV){ .speed = (Vec2){ 0, -0.01f } },
    &ecs
  );
  ecs__add_alpha_effect(mist, &ecs);
  ecs__add_draw(
    mist,
    (Draw){
      .shader = &mist_shader,
      .textures = MIST_TEXTURE,
      .mesh = &MIST_MESH,
      .draw = ecs__draw_mesh
    },
    &ecs
  );
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
}