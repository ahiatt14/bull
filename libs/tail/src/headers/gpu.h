#ifndef __FOOL_RENDERER__
#define __FOOL_RENDERER__

#include <stdint.h>

#include "mesh.h"
#include "m4x4.h"
#include "m3x3.h"
#include "vector.h"

struct Texture {
  unsigned int _impl_id;
  unsigned char *data;
  const char *name;
  int width;
  int height;
  int channel_count;
};

struct PointBuffer {
  unsigned int _impl_vbo_id;
  unsigned int _impl_vao_id;
  unsigned int points_length;
  struct Vec3 *points;
};

struct Shader {
  unsigned int _frag_impl_id;
  unsigned int _vert_impl_id;
  unsigned int _geo_impl_id;
  unsigned int _impl_id;
  const char *frag_src;
  const char *vert_src;
  const char *geo_src;
};

struct GPU {

  void (*clear)(
    struct Vec3 const *const color
  );
  void (*clear_depth_buffer)();
  void (*enable_depth_test)();
  void (*enable_blending)();
  void (*set_default_blending)();
  void (*disable_MSAA)();
  void (*enable_MSAA)();

  void (*cull_back_faces)();
  void (*cull_no_faces)();  

  void (*copy_dynamic_mesh_to_gpu)(
    struct DrawableMesh *const dm
  );
  void (*copy_static_mesh_to_gpu)(
    struct DrawableMesh *const dm
  );
  void (*copy_points_to_gpu)(
    struct PointBuffer *const pb
  );
  void (*update_gpu_mesh_data)(
    struct DrawableMesh const *const dm
  );

  void (*copy_texture_to_gpu)(
    struct Texture *const tex
  );

  void (*copy_shader_to_gpu)(
    struct Shader *const gpup
  );
  void (*copy_geo_stage_to_gpu)(
    const char *geo_src,
    struct Shader *const gpup
  );
  void (*select_shader)(
    struct Shader const *const gpup
  );
  void (*select_texture)(
    struct Texture const *const tex
  );
  void (*select_textures)(
    struct Shader const *const shad,
    uint8_t texture_count,
    struct Texture const *const *const textures
  );

  void (*set_viewport)(
    int x,
    int y,
    int w,
    int h
  );
  int (*get_viewport_height)();
  int (*get_viewport_width)();

  void (*set_shader_m3x3)(
    struct Shader const *const gpup,
    char const *name,
    struct M3x3 const *const value
  );
  void (*set_shader_m4x4)(
    struct Shader const *const gpup,
    char const *name,
    struct M4x4 const *const value
  );
  void (*set_shader_vec2)(
    struct Shader const *const gpup,
    char const *name,
    struct Vec2 value
  );  
  void (*set_shader_vec3)(
    struct Shader const *const gpup,
    char const *name,
    struct Vec3 value
  );
  void (*set_shader_float)(
    struct Shader const *const gpup,
    char const *name,
    float value
  );

  void (*draw_mesh)(
    struct DrawableMesh const *const dm
  );
  void (*draw_wireframe)(
    struct DrawableMesh const *const dm
  );
  void (*draw_points)(
    struct PointBuffer const *const pb
  );
};

void gpu__create_api(
  struct GPU *const gpu
);

#endif