#ifndef __TAIL_MESH__
#define __TAIL_MESH__

#include <stddef.h>

#include "vec3.h"

struct vec2 {
  float x, y;
};

struct vertex {
  struct vec3 position;
  struct vec3 normal;
  struct vec2 uv;
};

struct drawable_mesh {
  struct vertex *vertex_buffer;
  unsigned int *index_buffer;
  size_t vertex_buffer_size;
  size_t index_buffer_size;
  int index_buffer_length;
  unsigned int _impl_vbo_id;
  unsigned int _impl_ibo_id;
  unsigned int _impl_vao_id;
};

#endif