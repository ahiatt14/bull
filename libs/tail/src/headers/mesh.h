#ifndef __TAIL_MESH__
#define __TAIL_MESH__

#include <stddef.h>

#include "vec3.h"

typedef struct VEC2 {
  float x, y;
} vec2;

typedef struct VERTEX {
  vec3 position;
  vec3 normal;
  vec2 uv;
} vertex;

typedef struct DRAWABLE_MESH {
  vertex *vertex_buffer;
  unsigned int *index_buffer;
  size_t vertex_buffer_size;
  size_t index_buffer_size;
  int index_buffer_length;
  unsigned int _impl_vbo_id;
  unsigned int _impl_ibo_id;
  unsigned int _impl_vao_id;
} drawable_mesh;

#endif