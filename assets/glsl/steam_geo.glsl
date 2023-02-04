#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform float seconds_since_activation;

float wobble = 0.3;

void main() {

  gl_Position = gl_in[0].gl_Position;
  gl_Position.x =
    gl_Position.x +
    sin(0.5 * gl_Position.y - seconds_since_activation) * wobble;
  EmitVertex();

  gl_Position = gl_in[1].gl_Position;
  gl_Position.x =
    gl_Position.x +
    sin(0.5 * gl_Position.y - seconds_since_activation) * wobble;
  EmitVertex();
  
  gl_Position = gl_in[2].gl_Position;
  gl_Position.x =
    gl_Position.x +
    sin(0.5 * gl_Position.y - seconds_since_activation) * wobble;
  EmitVertex();

  EndPrimitive();
}