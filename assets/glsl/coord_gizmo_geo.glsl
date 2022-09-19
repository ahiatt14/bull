#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

const float MAGNITUDE = 0.3;

// in mat4 mvp;

out vec3 color;

uniform vec3 up;
uniform vec3 right;
uniform vec3 forward;

uniform vec3 up_color;
uniform vec3 right_color;
uniform vec3 forward_color;

uniform mat4 model = mat4(
  vec4(1.0, 0.0, 0.0, 0.0),
  vec4(0.0, 1.0, 0.0, 0.0),
  vec4(0.0, 0.0, 1.0, 0.0),
  vec4(0.0, 0.0, 0.0, 1.0)
);

uniform mat4 view = mat4(
  vec4(1.0, 0.0, 0.0, 0.0),
  vec4(0.0, 1.0, 0.0, 0.0),
  vec4(0.0, 0.0, 1.0, 0.0),
  vec4(0.0, 0.0, 0.0, 1.0)
);

uniform mat4 projection = mat4(
  vec4(1.0, 0.0, 0.0, 0.0),
  vec4(0.0, 1.0, 0.0, 0.0),
  vec4(0.0, 0.0, 1.0, 0.0),
  vec4(0.0, 0.0, 0.0, 1.0)
);

void generate_line(vec3 axis, vec3 _color) {

  color = _color;

  gl_Position = gl_in[0].gl_Position;
  EmitVertex();

  gl_Position = (
    projection * view * model *
    (gl_in[0].gl_Position + vec4(axis, 1))
  );
  EmitVertex();

  EndPrimitive();
}

void main() {
  generate_line(up, up_color);
  generate_line(right, right_color);
  generate_line(forward, forward_color);
}