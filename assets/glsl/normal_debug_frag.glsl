#version 330 core

out vec4 FragColor;

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;

void main()
{
  FragColor = vec4(fs_in.normal, 1.0);
}