#version 330 core

uniform sampler2D surface_texture;

uniform vec3 light_dir = vec3(0, -1, 0);
uniform vec3 light_color = vec3(1, 1, 1);

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} fs_in;

out vec4 FragColor;

void main()
{
  vec3 material = texture(surface_texture, fs_in.tex_uv).rgb * 0.5;

  float incidence = max(dot(fs_in.normal, -light_dir), 0);

  // vec3 diffuse;
  // if (incidence > 0.3) {
  //   diffuse = light_color * 0.4;
  // } else if (incidence > 0.25) {
  //   diffuse = light_color * 0.15;
  // } else {
  //   diffuse = vec3(0, 0, 0);
  // }
  
  FragColor = vec4(material + incidence, 1);
}