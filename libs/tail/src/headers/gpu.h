#ifndef __FOOL_RENDERER__
#define __FOOL_RENDERER__

#include "mesh.h"
#include "m4x4.h"
#include "m3x3.h"
#include "vec3.h"

typedef struct TEXTURE {
  unsigned int _impl_id;
  unsigned char *data;
  int width;
  int height;
  int channels_count;
} texture;

typedef struct GPU_PROGRAM {
  unsigned int _frag_impl_id;
  unsigned int _vert_impl_id;
  unsigned int _impl_id;
  const char *frag_shader_src;
  const char *vert_shader_src;
} gpu_program;

typedef struct GPU_API {

  void (*clear)(const float *color);
  void (*enable_depth_test)();
  void (*cull_back_faces)();

  void (*copy_mesh_to_gpu)(drawable_mesh *dm);
  void (*copy_rgb_texture_to_gpu)(texture *tex);

  void (*copy_program_to_gpu)(gpu_program *gpup);
  void (*select_gpu_program)(const gpu_program *gpup);
  void (*select_texture)(const texture *tex);

  void (*set_viewport)(int x, int y, int w, int h);
  int (*get_viewport_height)();
  int (*get_viewport_width)();

  void (*set_vertex_shader_m3x3)(
    const gpu_program *gpup,
    const char *name,
    const m3x3 *value
  );
  void (*set_vertex_shader_m4x4)(
    const gpu_program *gpup,
    const char *name,
    const m4x4 *value
  );
  void (*set_fragment_shader_vec3)(
    const gpu_program *gpup,
    const char *name,
    const vec3 *value
  );
  void (*set_fragment_shader_float)(
    const gpu_program *gpup,
    const char *name,
    float value
  );

  void (*draw_mesh)(const drawable_mesh*);
} gpu_api;

const gpu_api* gpu__create_api();

#endif