#version 330 core

#define MAX_POINT_LIGHTS 12

uniform sampler2D surface_texture;

struct PointLight {
  vec3 position;
  vec3 color;
  vec2 attenuation; // x = LINEAR, y = QUADRATIC
};

uniform PointLight point_lights[MAX_POINT_LIGHTS];
uniform int point_light_count;

uniform vec3 ambient_color = vec3(1);
uniform float ambient_strength = 0;

uniform vec2 uv_scroll = vec2(0, 0);

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;

out vec4 FragColor;

vec3 calculate_diffuse(
  PointLight light,
  vec3 frag_pos
) {

  float frag_distance = distance(frag_pos, light.position);

  vec3 light_dir = -normalize(light.position - frag_pos);

  float incidence = max(dot(fs_in.normal, light_dir), 0);

  float attenuation = 1.0 / (
    (
      1.0 +
      light.attenuation.x * frag_distance +
      light.attenuation.y * (frag_distance * frag_distance)
    )
  );

  return light.color * incidence * attenuation;
}

void main()
{
  vec4 material = texture(
    surface_texture,
    fs_in.tex_uv + uv_scroll
  );

  vec3 diffuse = vec3(0);
  for (int i = 0; i < point_light_count; i++)
    diffuse += calculate_diffuse(point_lights[i], fs_in.world_frag_pos);
  
  FragColor = vec4(
    material.rgb * (diffuse + (ambient_color * ambient_strength)),
    material.a
  );
}