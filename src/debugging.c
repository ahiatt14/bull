#include "tail.h"

#include "debugging.h"

#include "tail_helpers.h"
#include "constants.h"

#include "coord_gizmo_vert.h"
#include "coord_gizmo_geo.h"
#include "coord_gizmo_frag.h"


static struct shader coord_gizmo_shader;
static struct drawable_mesh coord_gizmo_mesh = (struct drawable_mesh){
  .vertices = (struct vertex[3]){
    {{ 0, 0, 0 }, { 0, 1, 0 }, { 0, 0 }},
    {{ 0, 0, 0 }, { 0, 1, 0 }, { 0, 0 }},
    {{ 0, 0, 0 }, { 0, 1, 0 }, { 0, 0 }}
  },
  .indices = (unsigned int[3]){
    0, 1, 2
  },
  .vertices_size = sizeof(struct vertex) * 3,
  .indices_size = sizeof(unsigned int) * 3,
  .indices_length = 3
};

void coord_gizmo__copy_assets_to_gpu(
  struct gpu_api const *const gpu
) {
  coord_gizmo_shader.vert_src = coord_gizmo_vert_src;
  coord_gizmo_shader.geo_src = coord_gizmo_geo_src;
  coord_gizmo_shader.frag_src = coord_gizmo_frag_src;
  gpu->copy_shader_to_gpu(&coord_gizmo_shader);
  gpu->copy_static_mesh_to_gpu(&coord_gizmo_mesh);
}

void coord_gizmo__draw(
  struct camera const *const cam,
  struct gpu_api const *const gpu,
  struct coord_gizmo const *const gizmo
) {

  gpu->select_shader(&coord_gizmo_shader);

  static struct m4x4 translation;
  m4x4__translation(gizmo->position, &translation);
  gpu__set_mvp(&translation, &M3X3_IDENTITY, cam, &coord_gizmo_shader, gpu);

  gpu->set_shader_vec3(&coord_gizmo_shader, "up", gizmo->space.up);
  gpu->set_shader_vec3(&coord_gizmo_shader, "right", gizmo->space.right);
  gpu->set_shader_vec3(&coord_gizmo_shader, "forward", gizmo->space.forward);
  gpu->set_shader_vec3(&coord_gizmo_shader, "up_color", COLOR_GREEN);
  gpu->set_shader_vec3(&coord_gizmo_shader, "right_color", COLOR_RED);
  gpu->set_shader_vec3(&coord_gizmo_shader, "forward_color", COLOR_BLUE);

  gpu->draw_mesh(&coord_gizmo_mesh);
}