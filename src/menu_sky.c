#include "tail.h"

#include "menu_sky.h"
#include "gpu_helpers.h"
#include "constants.h"

#include "sky_plane_mesh.h"
#include "sky_frag.h"
// #include "default_vert.h"

// static struct gpu_program shader;
// static struct drawable_mesh mesh;
// static struct m4x4 local_to_world;
// static struct m3x3 normals_local_to_world;
// static struct transform trans = {
//   .position = { 0, 0, 1 },
//   .rotation_in_deg = { 0, 0, 0 },
//   .scale = 2
// };

// void load_sky(const struct gpu_api *gpu) {

//   mesh.vertex_buffer = sky_plane_vertices;
//   mesh.index_buffer = sky_plane_indices;
//   mesh.vertex_buffer_size = sizeof(sky_plane_vertices);
//   mesh.index_buffer_size = sizeof(sky_plane_indices);
//   mesh.index_buffer_length = sky_plane_index_count;

//   shader.frag_shader_src = sky_frag_src;
//   shader.vert_shader_src = default_vert_src;

//   gpu->copy_mesh_to_gpu(&mesh);
//   gpu->copy_program_to_gpu(&shader);
// }

// void warp_mesh(double delta_time, struct drawable_mesh *dm) {

// }

// void draw_sky(
//   const struct gpu_api *gpu,
//   const struct m4x4 *view,
//   const struct m4x4 *perspective
// ) {
//   gpu->cull_back_faces();
//   gpu->select_gpu_program(&shader);
//   space__create_model(&WORLDSPACE, &trans, &local_to_world);
//   space__create_normals_model(&local_to_world, &normals_local_to_world);
//   gpu__set_mvp(
//     &local_to_world,
//     view,
//     perspective,
//     &normals_local_to_world,
//     &shader,
//     gpu
//   );
//   gpu->draw_mesh(&mesh);
// }