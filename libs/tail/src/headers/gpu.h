#ifndef __FOOL_RENDERER__
#define __FOOL_RENDERER__

#include <stdint.h>

#include "mesh.h"
#include "m4x4.h"
#include "m3x3.h"
#include "vector.h"

struct texture {
  unsigned int _impl_id;
  unsigned char *data;
  int width;
  int height;
  int channel_count;
};

struct shader {
  unsigned int _frag_impl_id;
  unsigned int _vert_impl_id;
  unsigned int _geo_impl_id;
  unsigned int _impl_id;
  const char *frag_src;
  const char *vert_src;
  const char *geo_src;
};

struct gpu_api {

  void (*clear)(struct vec3 const *const color);
  void (*clear_depth_buffer)();
  void (*enable_depth_test)();
  void (*enable_blending)();
  void (*set_default_blending)();
  void (*disable_MSAA)();
  void (*enable_MSAA)();

  void (*cull_back_faces)();
  void (*cull_no_faces)();  

  void (*copy_dynamic_mesh_to_gpu)(struct drawable_mesh *const dm);
  void (*copy_static_mesh_to_gpu)(struct drawable_mesh *const dm);
  void (*update_gpu_mesh_data)(struct drawable_mesh const *const dm);

  void (*copy_texture_to_gpu)(struct texture *const tex);

  void (*copy_shader_to_gpu)(struct shader *const gpup);
  void (*copy_geo_stage_to_gpu)(
    const char *geo_src,
    struct shader *const gpup
  );
  void (*select_shader)(struct shader const *const gpup);
  void (*select_texture)(struct texture const *const tex);
  void (*select_textures)(
    struct shader const *const shad,
    uint8_t texture_count,
    char const *const *const uniform_names,
    struct texture const *const *const textures
  );

  void (*set_viewport)(int x, int y, int w, int h);
  int (*get_viewport_height)();
  int (*get_viewport_width)();

  void (*set_shader_m3x3)(
    struct shader const *const gpup,
    char const *name,
    struct m3x3 const *const value
  );
  void (*set_shader_m4x4)(
    struct shader const *const gpup,
    char const *name,
    struct m4x4 const *const value
  );
  void (*set_shader_vec2)(
    struct shader const *const gpup,
    char const *name,
    struct vec2 value
  );  
  void (*set_shader_vec3)(
    struct shader const *const gpup,
    char const *name,
    struct vec3 value
  );
  void (*set_shader_float)(
    struct shader const *const gpup,
    char const *name,
    float value
  );

  void (*draw_mesh)(struct drawable_mesh const *const dm);
  void (*draw_wireframe)(struct drawable_mesh const *const dm);
};

void gpu__create_api(struct gpu_api *const gpu);

#endif