#version 330 core

uniform sampler2D surface_texture;

uniform float max_altitude = 1;
uniform vec3 bottom_color = vec3(1.0);

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
    bottom_color * 0.7,
    material,
    2 * normalized_altitude - 0.3
  );
  
  float surface_alpha = 1 - (1.7 * normalized_altitude - 0.4);

  FragColor = vec4(
    faded,
    surface_alpha
  );
}