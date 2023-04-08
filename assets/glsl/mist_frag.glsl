#version 330 core

#define MAX_POINT_LIGHTS 20

struct PointLight {
  vec3 position;
  vec3 color;
  vec2 attenuation; // x = LINEAR, y = QUADRATIC
};

vec3 calculate_diffuse(
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

float brightness(vec3 rgb) {
  return (rgb.r + rgb.g + rgb.b) / 3.0;
}

uniform sampler2D surface_texture;

uniform float seconds_since_activation;
uniform float limit_in_seconds;

uniform PointLight point_lights[MAX_POINT_LIGHTS];
uniform int point_count;

uniform vec3 ambient_color = vec3(1);
uniform float ambient_strength = 0;

// TODO: see if we can't pass whole structs
uniform vec3 skylight_color = vec3(1);
uniform vec3 skylight_direction = vec3(0, -1, 0);
uniform float skylight_strength = 0;

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;

out vec4 FragColor;

void main()
{
  vec4 material = texture(
    surface_texture,
    fs_in.tex_uv
  );

  vec3 diffuse = vec3(0);
  for (int i = 0; i < point_count; i++)
    diffuse += calculate_diffuse(point_lights[i], fs_in.world_frag_pos);

  diffuse += skylight_color * skylight_strength * 0.5;
  diffuse += ambient_color * ambient_strength * 0.2;

  float alpha = material.a - (0.8 - brightness(material.rgb));

  float ratio = seconds_since_activation /limit_in_seconds;
  alpha -= smoothstep(
    0,
    1,
    abs(0.5 - ratio)
  ) * 1.5;

  FragColor = vec4(
    material.rgb * diffuse,
    alpha
  );
}