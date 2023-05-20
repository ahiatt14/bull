#version 410 core

#define MAX_POINT_LIGHTS 20

struct PointLight {
  vec3 position;
  vec3 color;
  vec2 attenuation; // x = LINEAR, y = QUADRATIC
};

float calculate_incidence(
  vec3 light_direction,
  vec3 normal
) {
  return max(dot(normal, light_direction), 0);
}

vec3 calculate_point_light_diffuse(
  PointLight light,
  vec3 frag_pos
) {

  float frag_distance = distance(frag_pos, light.position);

  float attenuation = 1.0 / (
    (
      1.0 +
      light.attenuation.x * frag_distance +
      light.attenuation.y * (frag_distance * frag_distance)
    )
  );

  return light.color * attenuation;
}

uniform PointLight point_lights[MAX_POINT_LIGHTS];
uniform int point_count = 0;

uniform vec3 ambient_color = vec3(1);
uniform float ambient_strength = 0;

uniform vec3 skylight_color = vec3(1);
uniform vec3 skylight_direction = vec3(0, -1, 0);
uniform float skylight_strength = 0;

uniform vec2 uv_scroll = vec2(0, 0);

uniform sampler2D surface_texture;

in TES_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;

out vec4 FragColor;

void main()
{
  vec3 material = texture(surface_texture, fs_in.tex_uv).rgb;

  vec3 diffuse = vec3(0);
  for (int i = 0; i < point_count; i++)
    diffuse += calculate_point_light_diffuse(point_lights[i], fs_in.world_frag_pos);

  float skylight_incidence =
    calculate_incidence(-skylight_direction, fs_in.normal);
  diffuse += skylight_color * skylight_strength * skylight_incidence;
  
  FragColor = vec4(
    material.rgb * (diffuse + (ambient_color * ambient_strength)),
    1.0
  );
}