#version 330 core

uniform vec3 light_dir = vec3(
  0,
  -1,
  0
);

uniform vec3 core_color = vec3(
  1,
  1,
  1
);

uniform vec3 edge_color = vec3(
  0,
  0,
  0
);

in VS_OUT {
  vec3 mvp_frag_pos;
  vec3 mv_normal;
  vec2 tex_uv;
  mat4 projection;
} fs_in;

out vec4 FragColor;

void main() {

  vec3 color =
    mix(
      core_color,
      edge_color,
      max(dot(fs_in.mv_normal, -light_dir), 0)
    );

  FragColor = vec4(
    color,
    1
  );
}