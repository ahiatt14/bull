#include "tail.h"

#include "firing_guide.h"

#include "constants.h"
#include "tail_helpers.h"

#include "firing_guide_frag.h"
#include "default_vert.h"

static struct shader guide_shader;
static struct m4x4 guide_local_to_world;
static struct transform guide_transform = (struct transform){
  .position = {0,-0.1f,0},
  .rotation_in_deg = {-90,0,0},
  .scale = 11
};

void firing_guide__copy_assets_to_gpu(
  struct gpu_api const *const gpu
) {
  guide_shader.frag_src = firing_guide_frag_src;
  guide_shader.vert_src = default_vert_src;
  gpu->copy_shader_to_gpu(&guide_shader);
  space__create_model(&WORLDSPACE, &guide_transform, &guide_local_to_world);
}

void firing_guide__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct vec3 player_position
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
    COLOR_MAGENTA_WHITE
  );
  gpu->set_shader_float(
    &guide_shader,
    "player_radius_world",
    vec3__distance(
      player_position,
      ORIGIN
    )
  );
  gpu->draw_mesh(&QUAD);
}