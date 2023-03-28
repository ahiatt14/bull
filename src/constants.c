#include "tail.h"

#include "constants.h"

M3x3 M3X3_IDENTITY = (M3x3){{
  1, 0, 0,
  0, 1, 0,
  0, 0, 1
}};
M4x4 M4X4_IDENTITY = (M4x4){{
  1, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1, 0,
  0, 0, 0, 1
}};

Shader FLAT_TEXTURE_SHADER;
Shader SOLID_COLOR_SHADER;
Shader NORMALS_COLOR_SHADER;
Shader NORMALS_VIS_SHADER;
Shader DEFAULT_BILLBOARD_SHADER;
Shader SKYBOX_SHADER;

PointBuffer POINT = (PointBuffer){
  .points = (Vec3[1]){{ 0, 0, 0 }},
  .points_length = 1
};

DrawableMesh QUAD = (DrawableMesh){
  .vertices = (Vertex[4]){
    {{ -0.5f, 0, 0.5f }, { 0, 1, 0 }, { 0, 0 }},
    {{ 0.5f, 0, 0.5f }, { 0, 1, 0 }, { 1, 0 }},
    {{ 0.5f, 0, -0.5f }, { 0, 1, 0 }, { 1, 1 }},
    {{ -0.5f, 0, -0.5f }, { 0, 1, 0 }, { 0, 1 }}
  },
  .indices = (unsigned int[6]){
    0, 1, 2,
    0, 2, 3
  },
  .vertices_size = sizeof(Vertex) * 4,
  .indices_size = sizeof(unsigned int) * 6,
  .indices_length = 6
};

const Vec3 ORIGIN = { 0, 0, 0 };
const CoordinateSpace WORLDSPACE = {
  .up = { 0, 1, 0 },
  .right = { 1, 0, 0 },
  .forward = { 0, 0, -1 },
};