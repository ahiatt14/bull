#version 410 core

uniform samplerCube skybox;

uniform mat3 rotation = mat3(
  vec3(1, 0, 0),
  vec3(0, 1, 0),
  vec3(0, 0, 1)
);

in vec3 TexCoords;

out vec4 FragColor;

void main()
{
  vec3 rotated_tex_coords = rotation * TexCoords;
  FragColor = texture(
    skybox,
    vec3(
      rotated_tex_coords.x,
      -rotated_tex_coords.y,
      rotated_tex_coords.z
    )
  );
}