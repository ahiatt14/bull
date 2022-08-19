#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

const float MAGNITUDE = 0.3;

in VS_OUT {
  vec3 rvp_normal;
} gs_in[];

void generate_line(int i) {

  gl_Position = gl_in[i].gl_Position;
  EmitVertex();

  gl_Position =
    gl_in[i].gl_Position +
    vec4(gs_in[i].rvp_normal, 0) * MAGNITUDE;
  EmitVertex();

  EndPrimitive();
}

void main() {
  generate_line(0);
  generate_line(1);
  generate_line(2);
}