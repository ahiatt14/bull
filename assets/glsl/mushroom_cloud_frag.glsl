#version 330 core

uniform sampler2D surface_texture;

uniform float seconds_since_activation;
uniform float limit_in_seconds;

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;

out vec4 FragColor;

void main()
{
  vec3 material = texture(surface_texture, fs_in.tex_uv).rgb;

  float turning_to_smoke = 1.0 - seconds_since_activation / limit_in_seconds;
  
  FragColor = vec4(material * turning_to_smoke, 1.0);
}