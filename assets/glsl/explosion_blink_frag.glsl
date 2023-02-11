#version 330 core

out vec4 FragColor;
out float gl_FragDepth;

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;

uniform sampler2D tex;
uniform float seconds_since_activation;
uniform float limit_in_seconds;

void main()
{
  vec4 texture_color = texture(tex, fs_in.tex_uv);

  float fade = log(seconds_since_activation / limit_in_seconds) + 1.0;

  if (texture_color.a < fade) discard;

  gl_FragDepth = 0.0;
  FragColor = texture_color;
}