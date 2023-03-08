#version 410 core

in TES_OUT {
  vec3 normal;
  vec2 tex_uv;
} fs_in;

uniform vec3 light_dir = vec3(-1, -0.2, 0);
uniform vec3 light_color = vec3(1, 1, 1);
const float light_strength = 1;

const vec3 ambient_light = vec3(1);
const float ambient_strength = 0.1;

uniform sampler2D surface_texture;

out vec4 FragColor;

void main()
{
  vec3 material = texture(surface_texture, fs_in.tex_uv).rgb;

  float incidence = max(dot(fs_in.normal, -normalize(light_dir)), 0);

  vec3 light = mix(
    incidence * 2 + light_color * light_strength,
    ambient_light * ambient_strength,
    1.0 - incidence
  );
  
  FragColor = vec4(
    material * light,
    1
  );
}