#version 330 core

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;

uniform sampler2D mist_texture;

uniform vec2 uv_scroll;

uniform float altitude_center = 120;

out vec4 FragColor;

float brightness(vec3 color) {
  return (color.r + color.g + color.b) / 3.0;
}

void main()
{
  vec4 material = texture(
    mist_texture,
    fs_in.tex_uv + uv_scroll
  );

  float abs_altitude = abs(fs_in.world_frag_pos.y - altitude_center);
  float dist_coef = smoothstep(0, altitude_center, abs_altitude);
  
  FragColor = vec4(
    material.rgb,
    material.a - dist_coef
  );
}