#ifndef __TAIL_MESH__
#define __TAIL_MESH__

#include <stddef.h>

#include "vector.h"

struct Vertex {
  struct Vec3 position;
  struct Vec3 normal;
  struct Vec2 uv;
};

struct DrawableMesh {
  struct Vertex *vertices;
  unsigned int *indices;
  size_t vertices_size;
  size_t indices_size;
  int vertices_length;
  int indices_length;
  unsigned int _impl_vbo_id;
  unsigned int _impl_ibo_id;
  unsigned int _impl_vao_id;
};

#endif