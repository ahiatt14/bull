#version 330 core

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;
out vec4 FragColor;

uniform float player_radius_worldspace;
uniform float arena_radius_worldspace;
uniform vec3 color = vec3(
  0,
  0,
  0
);

void main() {

  vec2 world_origin = vec2(0);

  float frag_radius_world = distance(
    fs_in.world_frag_pos.xz,
    world_origin
  );

  float alpha = 1.0 - smoothstep(
    0,
    0.1,
    abs(frag_radius_world - arena_radius_worldspace)
  );
  
  FragColor = vec4(
    color,
    alpha - 0.7
  );
}