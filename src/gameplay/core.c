#include "tail.h"
#include "core.h"

#include "gpu_helpers.h"
#include "constants.h"

#include "core_mesh.h"
#include "solid_color_frag.h"
#include "default_vert.h"

static struct shader core_shader;

static struct m4x4 shared_local_to_world;
static struct m3x3 shared_normals_local_to_world;

void core__copy_assets_to_gpu(
  struct gpu_api const *const gpu
) {
  core_shader.frag_shader_src = solid_color_frag_src;
  core_shader.vert_shader_src = default_vert_src;
  gpu->copy_shader_to_gpu(&core_shader);
  gpu->copy_static_mesh_to_gpu(&core_mesh);
}

void core__update(
  double delta_time,
  struct core_state *const core
) {

}

void core__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct core_state const *const core
) {
  gpu->select_shader(&core_shader);
  gpu->set_fragment_shader_vec3(
    &core_shader,
    "color",
    &COLOR_GOLDEN_YELLOW
  );
  space__create_model(
    &WORLDSPACE,
    &core->transform,
    &shared_local_to_world
  );
  space__create_normals_model(
    &shared_local_to_world,
    &shared_normals_local_to_world
  );
  gpu__set_mvp(
    &shared_local_to_world,
    &shared_normals_local_to_world,
    cam,
    &core_shader,
    gpu
  );
  gpu->draw_mesh(&core_mesh);
}