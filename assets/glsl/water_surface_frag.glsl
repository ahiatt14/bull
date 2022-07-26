#version 330 core

uniform sampler2D cloud_cover;
uniform sampler2D surface_texture;

uniform vec3 water_color = vec3(
  65.0/255.0,
  95.0/255.0,
  124.0/255.0
);
uniform vec3 light_dir = vec3(0, 0, -1);
uniform vec3 light_color = vec3(1, 1, 1);

in vec2 TexCoord;
in vec3 normal;
in vec3 frag_world_position;

out vec4 FragColor;

void main()
{
  vec3 material =
    water_color *
    (texture(surface_texture, TexCoord).rgb * 0.15 + 0.6);
  vec3 diffuse =
    light_color *
    texture(cloud_cover, TexCoord).rgb * 1.4;
  float albedo = 1 - max(dot(normal, -light_dir), 0);
  
  FragColor = vec4(material + diffuse * albedo, 1);
}