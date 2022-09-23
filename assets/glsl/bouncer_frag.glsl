#version 330 core

uniform vec3 light1_dir = vec3(1,0,0);
uniform vec3 light2_dir = vec3(-1,0,0);
uniform vec3 light1_color = vec3(1,0,0);
uniform vec3 light2_color = vec3(0,0,1);
uniform vec3 color = vec3(0,0,0);

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;

out vec4 FragColor;

void main() {

  vec3 diffuse =
    (color * 0.2) +
    light1_color * max(dot(fs_in.normal, -light1_dir), 0) +
    light2_color * max(dot(fs_in.normal, -light2_dir), 0);

  FragColor = vec4(
    diffuse,
    1
  );
}