#include <stdio.h> // TODO: remove

#include "tail.h"

#include "gpu_helpers.h"

#include "square.h"
#include "constants.h"

#include "default_vert.h"

static struct drawable_mesh shared_square_mesh = {
  .vertices = (struct vertex[]){
    {{ -0.5f, -0.5f, 0 },{ 0, 0, -1 },{ 0, 0 }},
    {{ 0.5f, -0.5f, 0 },{ 0, 0, -1 },{ 1, 0 }},
    {{ 0.5f, 0.5f, 0 },{ 0, 0, -1 },{ 1, 1 }},
    {{ -0.5f, 0.5f, 0 },{ 0, 0, -1 },{ 0, 1 }}
  },
  .indices = (unsigned int[]){
    0, 1, 2,
    0, 2, 3
  },
  .vertices_size = sizeof(struct vertex) * 4,
  .indices_size = sizeof(unsigned int) * 6,
  .indices_length = 6
};

static struct m4x4 shared_temp_model;
static struct m3x3 shared_temp_normals_model;

// TODO: I probably don't like this
static unsigned int gpu_has_mesh = 0;

void square__create(
  struct transform transform,
  struct texture texture,
  const char *frag_shader_src,
  struct square *mem,
  const struct gpu_api *gpu
) {

  mem->_transform = transform;
  
  mem->_shader.vert_shader_src = default_vert_src;
  mem->_shader.frag_shader_src = frag_shader_src;
  gpu->copy_program_to_gpu(&mem->_shader);

  mem->_mesh = &shared_square_mesh;
  if (!gpu_has_mesh) {
    gpu->copy_static_mesh_to_gpu(&shared_square_mesh);
    gpu_has_mesh = 1;
  }

  mem->_texture = texture;
  switch (texture.channels_count) {
    case 1:
      gpu->copy_mono_texture_to_gpu(&texture);
      break;
    case 3:
      gpu->copy_rgb_texture_to_gpu(&texture);
      break;
  }
}

void square__draw(
  const struct square *sq,
  const struct camera *cam,
  const struct gpu_api *gpu
) {
  gpu->cull_back_faces();
  gpu->select_gpu_program(&sq->_shader);
  gpu->select_texture(&sq->_texture);

  // TODO: don't do every tick
  space__create_model(
    &WORLDSPACE,
    &sq->_transform,
    &shared_temp_model
  );
  space__create_normals_model(
    &shared_temp_model,
    &shared_temp_normals_model
  );
  gpu__set_mvp(
    &shared_temp_model,
    camera__get_lookat(cam),
    camera__get_perspective(cam),
    &shared_temp_normals_model,
    &sq->_shader,
    gpu
  );

  gpu->draw_mesh(&shared_square_mesh);
}