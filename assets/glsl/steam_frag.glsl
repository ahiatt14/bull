#version 330 core

uniform vec3 color = vec3(
  0.9,
  0.9,
  0.9
);

uniform vec3 light_dir = vec3(
  0,
  -1,
  0
);

uniform vec3 light_color = vec3(
  1,
  0.93,
  1
);

uniform vec3 top_color = vec3(
  1,
  1,
  1
);

uniform vec3 water_reflect_color = vec3(
  1,
  0,
  0
);

const vec3 water_reflect_direction = vec3(
  0,
  -1,
  0
);

uniform float max_altitude = 1;
float normalized_altitude;

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;

out vec4 FragColor;

void main() {

  normalized_altitude = fs_in.world_frag_pos.y / max_altitude;

  vec3 diffuse =
    light_color *
    max(dot(fs_in.normal, -light_dir), 0) +
    water_reflect_color *
    max(dot(fs_in.normal, water_reflect_direction), 0) * 0.2f;

  vec3 mixed =
    mix(color + diffuse, top_color, normalized_altitude);

  FragColor = vec4(
    mixed,
    1
  );
}