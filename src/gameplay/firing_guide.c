#include <math.h>

#include "tail.h"

#include "firing_guide.h"

#include "constants.h"
#include "tail_helpers.h"

#include "firing_guide_frag.h"
#include "default_vert.h"

static struct Shader guide_shader;
static struct M4x4 guide_local_to_world;
static struct Transform guide_transform;

void firing_guide__copy_assets_to_gpu(
  struct GPU const *const gpu
) {
  guide_transform = (struct Transform){
    .position = {0,-0.1f,0},
    .rotation = quaternion__create(
      WORLDSPACE.right,
      -(M_PI * 0.5f) 
    ),
    .scale = 20
  };
  guide_shader.frag_src = firing_guide_frag_src;
  guide_shader.vert_src = default_vert_src;
  gpu->copy_shader_to_gpu(&guide_shader);
  space__create_model(&WORLDSPACE, &guide_transform, &guide_local_to_world);
}

void firing_guide__draw(
  struct Camera const *const cam,
  struct GPU const *const gpu,
  float arena_radius_worldspace,
  struct Vec3 player_position
) {
  gpu->select_shader(&guide_shader);
  gpu__set_mvp(
    &guide_local_to_world,
    &M3X3_IDENTITY,
    cam,
    &guide_shader,
    gpu
  );
  gpu->set_shader_vec3(
    &guide_shader,
    "color",
    COLOR_GREEN
  );
  gpu->set_shader_float(
    &guide_shader,
    "arena_radius_worldspace",
    arena_radius_worldspace
  );
  gpu->set_shader_vec3(
    &guide_shader,
    "player_pos_worldspace",
    player_position
  );
  gpu->draw_mesh(&QUAD);
}