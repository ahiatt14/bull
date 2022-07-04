#include "tail.h"

#include "square.h"

const struct drawable_mesh shared_square_mesh = {
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

struct m4x4 shared_square_local_to_world = {{
  1, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1, 0,
  0, 0, 0, 1
}};
struct m3x3 shared_square_normals_local_to_world = {{
  1, 0, 0,
  0, 1, 0,
  0, 0, 1
}};