#version 330 core

float brightness(vec3 rgb) {
  return (rgb.r + rgb.g + rgb.b) / 3.0;
}

uniform sampler2D cloud;

uniform vec3 ambient_color = vec3(1);
uniform float ambient_strength = 0;

uniform vec2 uv_scroll = vec2(0, 0);

uniform float min_distance = 0;
uniform float max_distance = 1000;

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;

out vec4 FragColor;

void main() {

  vec4 material = texture(cloud, fs_in.tex_uv + uv_scroll);

  float normalized_distance =
    max(
      0,
      (length(fs_in.world_frag_pos.xz) - min_distance) /
      (max_distance - min_distance)
    );

  float alpha = 1.0 - brightness(material.rgb) * 0.6 - normalized_distance;

  FragColor = vec4(
    material.rgb * ambient_color * ambient_strength,
    alpha
  );
}