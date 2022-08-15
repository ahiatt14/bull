#version 330 core

uniform vec3 light_dir = vec3(
  0,
  -1,
  0
);

uniform vec3 light_color = vec3(
  1,
  0.93,
  1
);

in vec3 normal;

out vec4 FragColor;

void main() {

  FragColor = vec4(
    light_color * max(dot(normal, -light_dir), 0),
    1
  );
}