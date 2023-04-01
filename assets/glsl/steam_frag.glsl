#version 330 core

float brightness(vec3 rgb) {
  return (rgb.r + rgb.g + rgb.b) / 3.0;
}

float calculate_incidence(
  vec3 light_direction,
  vec3 normal
) {
  return max(dot(normal, light_direction), 0);
}

uniform sampler2D mist;

uniform vec3 ambient_color = vec3(1);
uniform float ambient_strength = 0;

uniform vec3 skylight_color = vec3(1);
uniform vec3 skylight_direction = vec3(0, -1, 0);
uniform float skylight_strength = 0;

uniform float min_altitude = 100.0;
uniform float max_altitude = 400.0;

uniform vec2 uv_scroll = vec2(0, 0);

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;

out vec4 FragColor;

void main() {

  float normalized_altitude =
    max(
      0,
      (fs_in.world_frag_pos.y - min_altitude) /
      (max_altitude - min_altitude)
    );

  vec4 material = texture(mist, fs_in.tex_uv + uv_scroll);

  float skylight_incidence =
    calculate_incidence(-skylight_direction, fs_in.normal);
  vec3 diffuse = skylight_color * skylight_strength * skylight_incidence;

  float alpha = 1.0 - normalized_altitude;

  FragColor = vec4(
    material.rgb * (diffuse + (ambient_color * ambient_strength)),
    alpha
  );
}