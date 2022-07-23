#version 330 core

uniform vec3 color = vec3(
  0.8,
  0.8,
  0.8
);
uniform vec3 light_dir = vec3(
  .707,
  -.707,
  0
);
uniform vec3 light_color = vec3(
  234.0/255.0,
  208.0/255.0,
  146.0/255.0
);

in vec2 TexCoord;
in vec3 normal;
in vec3 frag_world_position;

out vec4 FragColor;

void main()
{
  vec3 diffuse = light_color * max(dot(normal, -light_dir), 0.0);
  FragColor = vec4(
    color + diffuse * 0.5,
    1.0
  );
}