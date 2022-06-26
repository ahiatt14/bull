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
  struct vertex *vertices;
  unsigned int *indices;
  size_t vertices_size;
  size_t indices_size;
  int indices_length;
  unsigned int _impl_vbo_id;
  unsigned int _impl_ibo_id;
  unsigned int _impl_vao_id;
};

#endif