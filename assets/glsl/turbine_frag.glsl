#version 330 core

uniform float ratio_of_sunlight = 1;

uniform vec3 color = vec3(
  0.6,
  0.61,
  0.61
);
uniform vec3 light_dir = vec3(
  0,
  -1,
  0
);
uniform vec3 light_color = vec3(
  234.0/255.0,
  208.0/255.0,
  146.0/255.0
);

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;

out vec4 FragColor;

void main()
{

  vec3 diffuse =
    light_color *
    max(dot(fs_in.normal, -light_dir), 0);

  FragColor = vec4(
    (color + diffuse) * ratio_of_sunlight + 0.2,
    1.0
  );
}