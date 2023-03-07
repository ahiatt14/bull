#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 local_normal;
layout (location = 2) in vec2 uv;

out vec2 tex_uv;

void main()
{
  tex_uv = uv;
  gl_Position = vec4(position, 1.0);
}