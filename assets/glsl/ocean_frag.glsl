#version 330 core

in VS_OUT {
  vec3 normal;
  vec3 frag_world_pos;
  vec2 tex_uv;
} fs_in;

out vec4 FragColor;

uniform sampler2D tex;

uniform vec3 light_dir = vec3(0, -1, 0);
uniform vec3 light_color = vec3(1);
const float light_strength = 0.3;

uniform vec3 ambient_color = vec3(1);
const float ambient_strength = 0.1;

void main() {

  vec3 material = texture(tex, fs_in.tex_uv).xyz;

  float incidence = max(dot(fs_in.normal, -light_dir), 0);

  vec3 light = mix(
    incidence + light_color * light_strength,
    ambient_color * ambient_strength,
    1.0 - incidence
  );

  FragColor = vec4(
    material * light,
    1.0
  );
}