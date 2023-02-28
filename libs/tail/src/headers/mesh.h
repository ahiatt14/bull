#ifndef __TAIL_MESH__
#define __TAIL_MESH__

#include <stddef.h>

#include "vector.h"

typedef struct TAILVERTEX {
  Vec3 position;
  Vec3 normal;
  Vec2 uv;
} Vertex;

typedef struct TAILDRAWABLEMESH {
  Vertex *vertices;
  unsigned int *indices;
  size_t vertices_size;
  size_t indices_size;
  int vertices_length;
  int indices_length;
  unsigned int _impl_vbo_id;
  unsigned int _impl_ibo_id;
  unsigned int _impl_vao_id;
} DrawableMesh;

#endif