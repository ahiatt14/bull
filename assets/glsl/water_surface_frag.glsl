#version 330 core

uniform sampler2D cloud_cover;
uniform sampler2D surface_texture;

uniform vec3 water_color = vec3(
  65.0/255.0,
  95.0/255.0,
  124.0/255.0
);
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
  vec3 material =
    water_color *
    texture(surface_texture, fs_in.tex_uv).rgb;
  vec3 diffuse =
    light_color *
    texture(cloud_cover, fs_in.tex_uv).rgb;
  float albedo = max(dot(fs_in.normal, -light_dir), 0) + 0.8;
  
  FragColor = vec4(
    material,
    1
  );
}