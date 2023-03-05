#version 330 core

in VS_OUT {
  vec3 normal;
  vec3 frag_world_pos;
  vec2 tex_uv;
} fs_in;

out vec4 FragColor;

uniform sampler2D tex;

uniform vec3 color = vec3(0.5);

uniform vec3 light_dir = normalize(vec3(-1, -0.2, 0));
uniform vec3 light_color = vec3(1);
const float light_strength = 2;

uniform vec3 ambient_color = vec3(1);
const float ambient_strength = 0.1;

uniform vec3 camera_offset = vec3(0);

void main() {

  vec3 material = texture(
    tex,
    fs_in.tex_uv + vec2(
      camera_offset.x * 0.12,
      -camera_offset.z * 0.12
    )
  ).xyz * 0.6;

  float incidence = max(dot(fs_in.normal, -light_dir), 0);

  vec3 light = mix(
    incidence * 5 + light_color * light_strength,
    ambient_color * ambient_strength,
    1.0 - incidence
  );

  FragColor = vec4(
    material * light,
    1.0
  );
}