#ifndef __TAIL_default_vert_SRC__
#define __TAIL_default_vert_SRC__
const char *default_vert_src = ""
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 local_normal;\n"
"layout (location = 2) in vec2 uv;\n"
"uniform mat4 model = mat4(\n"
"  vec4(1.0, 0.0, 0.0, 0.0),\n"
"  vec4(0.0, 1.0, 0.0, 0.0),\n"
"  vec4(0.0, 0.0, 1.0, 0.0),\n"
"  vec4(0.0, 0.0, 0.0, 1.0)\n"
");\n"
"uniform mat4 lookat = mat4(\n"
"  vec4(1.0, 0.0, 0.0, 0.0),\n"
"  vec4(0.0, 1.0, 0.0, 0.0),\n"
"  vec4(0.0, 0.0, 1.0, 0.0),\n"
"  vec4(0.0, 0.0, 0.0, 1.0)\n"
");\n"
"uniform mat4 perspective = mat4(\n"
"  vec4(1.0, 0.0, 0.0, 0.0),\n"
"  vec4(0.0, 1.0, 0.0, 0.0),\n"
"  vec4(0.0, 0.0, 1.0, 0.0),\n"
"  vec4(0.0, 0.0, 0.0, 1.0)\n"
");\n"
"uniform mat3 normals_model = mat3(\n"
"  vec3(1.0, 0.0, 0.0),\n"
"  vec3(0.0, 1.0, 0.0),\n"
"  vec3(0.0, 0.0, 1.0)\n"
");\n"
"out vec3 normal;\n"
"out vec3 frag_world_position;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"  TexCoord = uv;\n"
"  normal = normalize(normals_model * local_normal);\n"
"  frag_world_position = vec3(model * vec4(position, 1.0));\n"
"  gl_Position = perspective * lookat * model * vec4(position, 1.0);\n"
"\n"
"}\n";
#endif