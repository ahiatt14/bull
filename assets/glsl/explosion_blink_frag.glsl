#version 330 core

out vec4 FragColor;
out float gl_FragDepth;

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;

uniform sampler2D tex;
uniform float lifespan_in_seconds;
uniform float total_elapsed_seconds;

void main()
{
  vec4 texture_color = texture(tex, fs_in.tex_uv);

  float lifespan_lived = total_elapsed_seconds / lifespan_in_seconds;

  if (texture_color.a < lifespan_lived) discard;

  gl_FragDepth = 0.0;
  FragColor = texture_color;
}