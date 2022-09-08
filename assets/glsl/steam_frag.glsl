#version 330 core

uniform sampler2D surface_texture;

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

  vec3 material = texture(surface_texture, fs_in.tex_uv).rgb;

  vec3 faded = mix(
    top_color,
    material,
    2 * normalized_altitude - 0.3
  );

  vec3 diffuse =
    light_color *
    max(dot(fs_in.normal, -light_dir), 0) * 0.7;

  vec3 mixed = faded + diffuse;
  
  float surface_alpha = 1 - (1.7 * normalized_altitude - 0.4);

  FragColor = vec4(
    mixed,
    surface_alpha
  );
}