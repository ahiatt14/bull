#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

const float MAGNITUDE = 0.3;

const vec3 green = vec3(0,1,0);
const vec3 red = vec3(1,0,0);
const vec3 blue = vec3(0,0,1);

out vec3 color;

uniform vec3 up;
uniform vec3 right;
uniform vec3 forward;

uniform mat4 view = mat4(
  vec4(1.0, 0.0, 0.0, 0.0),
  vec4(0.0, 1.0, 0.0, 0.0),
  vec4(0.0, 0.0, 1.0, 0.0),
  vec4(0.0, 0.0, 0.0, 1.0)
);

void generate_line(vec3 axis, vec3 color) {

  color = color;

  gl_Position = gl_in[0].gl_Position;
  EmitVertex();

  gl_Position = (
    view *
    (gl_in[0].gl_Position + vec4(axis, 1))
  ) * MAGNITUDE;
  EmitVertex();

  EndPrimitive();
}

void main() {
  // TODO: hardcoding the colors here bad
  generate_line(up, green);
  generate_line(right, red);
  generate_line(forward, blue);
}