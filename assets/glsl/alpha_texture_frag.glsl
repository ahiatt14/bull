#version 330 core

out vec4 FragColor;

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;

uniform sampler2D tex;

void main()
{
  FragColor = texture(tex, fs_in.tex_uv);
}