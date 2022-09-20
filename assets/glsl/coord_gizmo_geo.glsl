#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

const float MAGNITUDE = 6;

in VS_OUT {
  mat4 vp;
} gs_in[];

out vec3 color;

uniform vec3 up;
uniform vec3 right;
uniform vec3 forward;

uniform vec3 up_color;
uniform vec3 right_color;
uniform vec3 forward_color;

void generate_line(vec3 axis, vec3 _color) {

  color = _color;

  gl_Position = gl_in[0].gl_Position;
  EmitVertex();

  gl_Position = gs_in[0].vp * (gl_in[0].gl_Position + vec4(axis, 1.0));
  EmitVertex();

  EndPrimitive();
}

void main() {
  generate_line(up, up_color);
  generate_line(right, right_color);
  generate_line(forward, forward_color);
}