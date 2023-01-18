#version 330 core

uniform sampler2D surface_texture;

vec3 COLOR_WHITE = vec3(1.0);

uniform float lifespan_in_seconds;
uniform float total_elapsed_seconds;

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;

out vec4 FragColor;

void main()
{
  vec3 material = texture(surface_texture, fs_in.tex_uv).rgb;

  float fade = total_elapsed_seconds / lifespan_in_seconds;

  // vec3 mixed = mix(
  //   COLOR_WHITE,
  //   material,
  //   flash_to_flame
  // );
  
  FragColor = vec4(
    material,
    1.0 - fade
  );
}