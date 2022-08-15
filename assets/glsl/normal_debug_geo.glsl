#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

in vec3 normal;
in mat4 projection;

void main() {
  gl_Position = projection * gl_in[]
}