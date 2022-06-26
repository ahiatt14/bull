#ifndef __BULL_QUAD__
#define __BULL_QUAD__

#include "tail.h"

struct square {
  struct transform _transform;
  struct texture _texture;
  struct gpu_program _shader;
  const struct drawable_mesh *_mesh;
};

void square__create(
  struct transform trans,
  struct texture tex,
  const char *frag_shader_src,
  struct square *mem,
  const struct gpu_api *gpu
);

void square__draw(
  const struct square *sq,
  const struct camera *cam,
  const struct gpu_api *gpu
);

// void square__translate(float x, float y, float z, struct *square);

#endif