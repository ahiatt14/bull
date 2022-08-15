#version 330 core

uniform vec3 light_dir = vec3(
  -1,
  0,
  0
);

uniform vec3 light_color = vec3(
  .95,
  .9,
  .93
);

uniform vec3 night_color = vec3(
  0.4,
  0.56,
  0.63
);

in VS_OUT {
  vec3 mvp_frag_pos;
  vec3 mv_normal;
  vec2 tex_uv;
  mat4 projection;
} fs_in;

out vec4 FragColor;

void main() {

  vec3 diffuse =
    mix(
      night_color,
      light_color,
      max(dot(fs_in.mv_normal, -light_dir), 0)
    );

  FragColor = vec4(
    diffuse,
    1
  );
}