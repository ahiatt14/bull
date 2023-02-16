#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

in VS_OUT {
  vec4 endpoint;
} gs_in[];

void main() {

  gl_Position = gl_in[0].gl_Position;
  EmitVertex();

  gl_Position = gl_in[0].gl_Position + gs_in[0].endpoint;
  EmitVertex();

  EndPrimitive();
}