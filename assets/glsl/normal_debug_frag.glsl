#version 330 core

out vec4 FragColor;

in VS_OUT {
  vec3 mvp_frag_pos;
  vec3 mv_normal;
  vec2 tex_uv;
  mat4 projection;
} fs_in;

void main()
{
  FragColor = vec4(fs_in.mv_normal, 1.0);
}