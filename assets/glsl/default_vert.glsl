#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 local_normal;
layout (location = 2) in vec2 uv;
uniform mat4 model = mat4(
  vec4(1.0, 0.0, 0.0, 0.0),
  vec4(0.0, 1.0, 0.0, 0.0),
  vec4(0.0, 0.0, 1.0, 0.0),
  vec4(0.0, 0.0, 0.0, 1.0)
);
uniform mat4 view = mat4(
  vec4(1.0, 0.0, 0.0, 0.0),
  vec4(0.0, 1.0, 0.0, 0.0),
  vec4(0.0, 0.0, 1.0, 0.0),
  vec4(0.0, 0.0, 0.0, 1.0)
);
uniform mat4 perspective = mat4(
  vec4(1.0, 0.0, 0.0, 0.0),
  vec4(0.0, 1.0, 0.0, 0.0),
  vec4(0.0, 0.0, 1.0, 0.0),
  vec4(0.0, 0.0, 0.0, 1.0)
);
uniform mat3 normals_model = mat3(
  vec3(1.0, 0.0, 0.0),
  vec3(0.0, 1.0, 0.0),
  vec3(0.0, 0.0, 1.0)
);
out vec3 normal;
out vec3 frag_world_position;
out vec2 TexCoord;
void main()
{
  TexCoord = uv;
  normal = normalize(normals_model * local_normal);
  frag_world_position = vec3(model * vec4(position, 1.0));
  gl_Position = perspective * view * model * vec4(position, 1.0);
}