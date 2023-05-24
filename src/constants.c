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

const Vec3 ORIGIN = { 0, 0, 0 };
const CoordinateSpace WORLDSPACE = {
  .up = { 0, 1, 0 },
  .right = { 1, 0, 0 },
  .forward = { 0, 0, -1 },
};