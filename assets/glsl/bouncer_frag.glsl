#version 330 core

uniform vec3 light_dir = vec3(
  0,
  -1,
  0
);

uniform vec3 core_color = vec3(
  1,
  1,
  1
);

uniform vec3 edge_color = vec3(
  0,
  0,
  0
);

in vec3 normal;

out vec4 FragColor;

void main() {

  vec3 color =
    mix(
      core_color,
      edge_color,
      max(dot(normal, -light_dir), 0)
    );

  FragColor = vec4(
    color,
    1
  );
}