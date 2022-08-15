#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

const float MAGNITUDE = 0.2;

in VS_OUT {
  vec3 mvp_frag_pos;
  vec3 mv_normal;
  vec2 tex_uv;
  mat4 projection;
} gs_in[];

void generate_line(int i) {
  gl_Position = gs_in[i].projection * gl_in[i].gl_Position;
  EmitVertex();

  gl_Position = gs_in[i].projection * (
    gl_in[i].gl_Position +
    vec4(gs_in[i].mv_normal, 0) * MAGNITUDE
  );
  EmitVertex();

  EndPrimitive();
}

void main() {
  generate_line(0);
  generate_line(1);
  generate_line(2);
}