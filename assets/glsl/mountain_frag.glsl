#version 330 core

uniform sampler2D surface_texture;

uniform vec3 light_dir = vec3(0, -1, 0);
uniform vec3 light_color = vec3(1, 1, 1);
const float light_strength = 0.5;

const vec3 ambient_light = vec3(1);
const float ambient_strength = 0.5;

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;

out vec4 FragColor;

void main()
{
  vec3 material = texture(surface_texture, fs_in.tex_uv).rgb;

  float incidence = max(dot(fs_in.normal, -light_dir), 0);

  vec3 light = mix(
    incidence + light_color * light_strength,
    ambient_light * ambient_strength,
    1.0 - incidence
  );
  
  FragColor = vec4(
    material * light,
    1
  );
}