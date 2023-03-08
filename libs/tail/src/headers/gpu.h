#ifndef __FOOL_RENDERER__
#define __FOOL_RENDERER__

#include <stdint.h>

#include "mesh.h"
#include "m4x4.h"
#include "m3x3.h"
#include "vector.h"

#define FILTER__NEAREST 0
#define FILTER__LINEAR 1

#define WRAP__REPEAT 0
#define WRAP__MIRROR_REPEAT 1
#define WRAP__CLAMP 2

typedef struct TAILTEXTURE {
  unsigned int _impl_id;
  unsigned char *data;
  const char *name;
  int width;
  int height;
  int channel_count;
} Texture;

typedef struct TAILCUBEMAP {
  unsigned int _impl_id;
  // NOTE: ordered px, nx, py, ny, pz, nz
  Texture *sides[6];
} Cubemap;

typedef struct TAILPOINTBUFFER {
  unsigned int _impl_vbo_id;
  unsigned int _impl_vao_id;
  unsigned int points_length;
  Vec3 *points;
} PointBuffer;

typedef struct TAILSHADER {
  unsigned int _frag_impl_id;
  unsigned int _vert_impl_id;
  unsigned int _geo_impl_id;
  unsigned int _tess_ctrl_impl_id;
  unsigned int _tess_eval_impl_id;
  unsigned int _impl_id;
  const char *frag_src;
  const char *vert_src;
  const char *geo_src;
  const char *tess_ctrl_src;
  const char *tess_eval_src;
} Shader;

typedef struct TAILGPU {

  void (*clear)(
    Vec3 const *const color
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
    DrawableMesh *const mesh
  );
  void (*copy_static_mesh_to_gpu)(
    DrawableMesh *const mesh
  );
  void (*copy_tessellated_mesh_to_gpu)(
    DrawableMesh *const mesh
  );
  void (*copy_points_to_gpu)(
    PointBuffer *const buffer
  );
  void (*update_gpu_mesh_data)(
    DrawableMesh const *const mesh
  );

  void (*copy_texture_to_gpu)(
    uint8_t filter,
    uint8_t wrap,
    Texture *const tex
  );

  void (*copy_cubemap_to_gpu)(
    uint8_t filter,
    Cubemap *const cubemap
  );

  void (*copy_shader_to_gpu)(
    Shader *const shader
  );
  void (*copy_geo_stage_to_gpu)(
    const char *geo_src,
    Shader *const shader
  );
  void (*select_shader)(
    Shader const *const shader
  );

  void (*select_cubemap)(
    Cubemap const *const cubemap
  );
  void (*select_texture)(
    Texture const *const tex
  );
  void (*select_textures)(
    Shader const *const shad,
    uint8_t texture_count,
    Texture const *const *const textures
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
    Shader const *const shader,
    char const *name,
    M3x3 const *const value
  );
  void (*set_shader_m4x4)(
    Shader const *const shader,
    char const *name,
    M4x4 const *const value
  );
  void (*set_shader_vec2)(
    Shader const *const shader,
    char const *name,
    Vec2 value
  );  
  void (*set_shader_vec3)(
    Shader const *const shader,
    char const *name,
    Vec3 value
  );
  void (*set_shader_int)(
    Shader const *const shader,
    char const *name,
    int value
  );
  void (*set_shader_float)(
    Shader const *const shader,
    char const *name,
    float value
  );

  void (*draw_mesh)(
    DrawableMesh const *const mesh
  );
  void (*draw_tessellated_mesh)(
    DrawableMesh const *const mesh
  );
  void (*draw_tessellated_wireframe)(
    DrawableMesh const *const mesh
  );
  void (*draw_wireframe)(
    DrawableMesh const *const mesh
  );
  void (*draw_points)(
    PointBuffer const *const buffer
  );
} GPU;

void gpu__create_api(
  GPU *const gpu
);

#endif