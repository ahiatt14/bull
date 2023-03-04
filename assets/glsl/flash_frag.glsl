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
  vec4 color = texture(tex, fs_in.tex_uv);

  float ratio = seconds_since_activation / limit_in_seconds;

  gl_FragDepth = 0.0;
  FragColor = vec4(
    color.rgb,
    color.a - ratio
  );
}