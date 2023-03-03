#version 330 core

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;
out vec4 FragColor;

const float MAX_PLAYER_TO_FRAG_DISTANCE = 3;
const float MISSILE_GUIDE_HALF_WIDTH = 2;
const float LINE_THICKNESS = 0.05;

vec2 WORLD_ORIGIN = vec2(0);

uniform vec3 player_pos_worldspace;
uniform float arena_radius_worldspace;

uniform vec3 color = vec3(1, 0, 0);

void main() {

  vec2 player_to_origin = WORLD_ORIGIN - player_pos_worldspace.xz;
  vec2 player_to_origin_perpendicular = vec2(player_to_origin.y, -player_to_origin.x);
  vec2 frag_pos_to_origin = WORLD_ORIGIN - fs_in.world_frag_pos.xz;
  float frag_distance_to_player_origin_line =
    abs(dot(normalize(player_to_origin_perpendicular), frag_pos_to_origin));

  float player_radius = distance(
    player_pos_worldspace.xz,
    WORLD_ORIGIN
  );
  float frag_radius = distance(
    fs_in.world_frag_pos.xz,
    WORLD_ORIGIN
  );

  float line_alpha = 0.9 - smoothstep(
    0,
    0.06,
    frag_distance_to_player_origin_line
  );

  float player_radius_alpha = 0.9 - smoothstep(
    0,
    0.06,
    abs(frag_radius - player_radius)
  );

  float arena_radius_alpha = 0.9 - smoothstep(
    0,
    0.06,
    abs(arena_radius_worldspace - frag_radius)
  );
  
  float total_alpha = line_alpha + player_radius_alpha + arena_radius_alpha;

  FragColor = vec4(
    color + total_alpha * total_alpha,
    total_alpha
  );
}