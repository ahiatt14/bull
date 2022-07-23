#include "tail.h"

#include "turbine.h"
#include "constants.h"
#include "bull_math.h"
#include "gpu_helpers.h"

#include "turbine_base_mesh.h"
#include "turbine_blades_mesh.h"

#include "turbine_frag.h"
#include "solid_color_frag.h"
#include "normal_debug_frag.h"
#include "default_vert.h"

static const struct vec3 TURBINE_BLADE_OFFSET = { 0, 2.25f, -0.2f };
static struct transform shared_blades_transform;

static struct m4x4 shared_local_to_world;
static struct m3x3 shared_normals_local_to_world;

static struct shader shared_turbine_shader;

void turbine__copy_assets_to_gpu(
  struct gpu_api const *const gpu
) {
  shared_turbine_shader.frag_shader_src = turbine_frag_src;
  shared_turbine_shader.vert_shader_src = default_vert_src;
  gpu->copy_shader_to_gpu(&shared_turbine_shader);
  gpu->copy_static_mesh_to_gpu(&turbine_base_mesh);
  gpu->copy_static_mesh_to_gpu(&turbine_blades_mesh);
}

void turbine__spin_blades(
  double delta_time,
  float degrees_per_second,
  struct turbine *const turb
) {
  turb->blades_rotation_in_deg += degrees_per_second * delta_time;
}

void turbine__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct turbine const *const turb
) {
  shared_blades_transform = (struct transform){
    vec3_plus_vec3(
      turb->transform.position,
      scalar_x_vec3(turb->transform.scale, TURBINE_BLADE_OFFSET)
    ),
    { 0, turb->transform.rotation_in_deg.y, turb->blades_rotation_in_deg },
    turb->transform.scale
  };

  gpu->cull_back_faces();
  gpu->select_shader(&shared_turbine_shader);
  space__create_model(
    &WORLDSPACE,
    &turb->transform,
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
    &shared_turbine_shader,
    gpu
  );
  gpu->draw_mesh(&turbine_base_mesh);
  space__create_model(
    &WORLDSPACE,
    &shared_blades_transform,
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
    &shared_turbine_shader,
    gpu
  );
  gpu->draw_mesh(&turbine_blades_mesh);
}
