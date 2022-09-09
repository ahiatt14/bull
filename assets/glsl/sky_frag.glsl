#version 330 core

// uniform vec3 light_dir = vec3(
//   -1,
//   0,
//   0
// );

float max_altitude = 4;

uniform sampler2D sky_texture;

uniform vec3 horizon_color = vec3(
  .95,
  .9,
  .93
);

uniform vec3 night_color = vec3(
  0, 0, 0
);

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;

out vec4 FragColor;

void main() {

  float normalized_altitude = fs_in.world_frag_pos.y / max_altitude;

  vec3 material = texture(sky_texture, fs_in.tex_uv).rgb;

  vec3 mixed =
    mix(
      horizon_color,
      material,
      normalized_altitude - 0.6
    );

  FragColor = vec4(
    mixed,
    1
  );
}