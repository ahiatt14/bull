#version 330 core

out vec4 FragColor;

in VS_OUT {
  vec3 mvp_frag_pos;
  vec3 mv_normal;
  vec2 tex_uv;
  mat4 projection;
} fs_in;

uniform sampler2D tex;

void main()
{
  FragColor = texture(tex, fs_in.tex_uv);
}