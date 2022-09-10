#version 330 core

uniform vec3 color = vec3(
  0.12,
  0.1,
  0.14
);
uniform vec3 light_dir = vec3(
  -.707,
  -.707,
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
  FragColor = vec4(
    color + light_color * max(dot(fs_in.normal, -light_dir), 0.0) * 0.5,
    0.3
  );
}