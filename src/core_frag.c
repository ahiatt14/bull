#include "core_frag.h"
const char *core_frag_src = "#version 330 core\n"
"\n"
"uniform vec3 color = vec3(\n"
"  0.12,\n"
"  0.1,\n"
"  0.14\n"
");\n"
"uniform vec3 light_dir = vec3(\n"
"  -.707,\n"
"  -.707,\n"
"  0\n"
");\n"
"uniform vec3 light_color = vec3(\n"
"  234.0/255.0,\n"
"  208.0/255.0,\n"
"  146.0/255.0\n"
");\n"
"\n"
"in vec2 TexCoord;\n"
"in vec3 normal;\n"
"in vec3 frag_world_position;\n"
"\n"
"out vec4 FragColor;\n"
"\n"
"void main()\n"
"{\n"
"  FragColor = vec4(\n"
"    color + light_color * max(dot(normal, -light_dir), 0.0) * 0.5,\n"
"    1.0\n"
"  );\n"
"\n"
"}\n";
