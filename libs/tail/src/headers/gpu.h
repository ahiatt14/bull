#ifndef __FOOL_RENDERER__
#define __FOOL_RENDERER__

#include "mesh.h"
#include "m4x4.h"
#include "m3x3.h"
#include "vector.h"

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

  void (*clear)(struct vec3 const *const color);
  void (*clear_depth_buffer)();
  void (*enable_depth_test)();
  void (*cull_back_faces)();
  void (*cull_no_faces)();

  void (*copy_dynamic_mesh_to_gpu)(struct drawable_mesh *const dm);
  void (*copy_static_mesh_to_gpu)(struct drawable_mesh *const dm);
  void (*update_gpu_mesh_data)(struct drawable_mesh const *const dm);

  void (*copy_rgb_texture_to_gpu)(struct texture *const tex);
  void (*copy_mono_texture_to_gpu)(struct texture *const tex);

  void (*copy_program_to_gpu)(struct gpu_program *const gpup);
  void (*select_gpu_program)(struct gpu_program const *const gpup);
  void (*select_texture)(struct texture const *const tex);

  void (*set_viewport)(int x, int y, int w, int h);
  int (*get_viewport_height)();
  int (*get_viewport_width)();

  void (*set_vertex_shader_m3x3)(
    struct gpu_program const *const gpup,
    char const *name,
    struct m3x3 const *const value
  );
  void (*set_vertex_shader_m4x4)(
    struct gpu_program const *const gpup,
    char const *name,
    struct m4x4 const *const value
  );
  void (*set_fragment_shader_vec3)(
    struct gpu_program const *const gpup,
    char const *name,
    struct vec3 const *const value
  );
  void (*set_fragment_shader_float)(
    struct gpu_program const *const gpup,
    char const *name,
    float value
  );

  void (*draw_mesh)(struct drawable_mesh const *const dm);
};

void gpu__create_api(struct gpu_api *const gpu);

#endif