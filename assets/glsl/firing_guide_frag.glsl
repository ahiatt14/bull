#version 330 core

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;
out vec4 FragColor;

uniform float player_radius_world;
uniform vec3 color = vec3(
  0,
  0,
  0
);

void main() {

  // TODO: could parameterize but don't imagine
  // we'll need it
  vec2 world_origin = vec2(0);

  // float frag_radius = dot(
  //   norm_frag_vwprt_pos - vec2(0.5),
  //   norm_frag_vwprt_pos - vec2(0.5)
  // ) * 4.0;

  float frag_radius_world = distance(fs_in.world_frag_pos.xz, world_origin);

  float alpha = 1.0 - smoothstep(
    0,
    0.1,
    abs(frag_radius_world - player_radius_world)
  );
  
  FragColor = vec4(
    color,
    alpha - 0.7
  );
}