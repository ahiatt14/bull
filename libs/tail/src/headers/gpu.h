#ifndef __FOOL_RENDERER__
#define __FOOL_RENDERER__

#include "mesh.h"
#include "m4x4.h"
#include "m3x3.h"
#include "vec3.h"

struct texture {
  unsigned int _impl_id;
  unsigned char *data;
  int width;
  int height;
  int channels_count;
};

struct gpu_program {
  unsigned int _frag_impl_id;
  unsigned int _vert_impl_id;
  unsigned int _impl_id;
  const char *frag_shader_src;
  const char *vert_shader_src;
};

struct gpu_api {

  void (*clear)(const struct vec3 *color);
  void (*clear_depth_buffer)();
  void (*enable_depth_test)();
  void (*cull_back_faces)();
  void (*cull_no_faces)();

  void (*copy_dynamic_mesh_to_gpu)(struct drawable_mesh *dm);
  void (*copy_static_mesh_to_gpu)(struct drawable_mesh *dm);
  void (*update_gpu_mesh_data)(const struct drawable_mesh *dm);

  void (*copy_rgb_texture_to_gpu)(struct texture *tex);
  void (*copy_mono_texture_to_gpu)(struct texture *tex);

  void (*copy_program_to_gpu)(struct gpu_program *gpup);
  void (*select_gpu_program)(const struct gpu_program *gpup);
  void (*select_texture)(const struct texture *tex);

  void (*set_viewport)(int x, int y, int w, int h);
  int (*get_viewport_height)();
  int (*get_viewport_width)();

  void (*set_vertex_shader_m3x3)(
    const struct gpu_program *gpup,
    const char *name,
    const struct m3x3 *value
  );
  void (*set_vertex_shader_m4x4)(
    const struct gpu_program *gpup,
    const char *name,
    const struct m4x4 *value
  );
  void (*set_fragment_shader_vec3)(
    const struct gpu_program *gpup,
    const char *name,
    const struct vec3 *value
  );
  void (*set_fragment_shader_float)(
    const struct gpu_program *gpup,
    const char *name,
    float value
  );

  void (*draw_mesh)(const struct drawable_mesh *dm);
};

void gpu__create_api(struct gpu_api *gpu);

#endif