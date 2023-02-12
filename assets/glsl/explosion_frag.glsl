#version 330 core

uniform sampler2D surface_texture;

vec3 COLOR_WHITE = vec3(1.0);

uniform vec2 total_uv_scroll;
uniform float seconds_since_activation;
uniform float limit_in_seconds;

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;

out vec4 FragColor;

float brightness(vec3 color) {
  return (color.r + color.g + color.b) / 3.0;
}

void main()
{
  vec3 material = texture(
    surface_texture,
    fs_in.tex_uv + total_uv_scroll
  ).rgb;
  float ratio = seconds_since_activation / limit_in_seconds;

  float fade = log(ratio) + 1.0;

  if (
    brightness(material) < 0.6 ||
    brightness(material) < fade
  ) discard;

  // vec3 mixed = mix(
  //   COLOR_WHITE,
  //   material,
  //   flash_to_flame
  // );
  
  FragColor = vec4(
    material,
    1.0
  );
}