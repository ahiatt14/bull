#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 local_normal;
layout (location = 2) in vec2 uv;

uniform vec3 center_pos_world;
uniform float scale;

uniform mat4 view = mat4(
  vec4(1.0, 0.0, 0.0, 0.0),
  vec4(0.0, 1.0, 0.0, 0.0),
  vec4(0.0, 0.0, 1.0, 0.0),
  vec4(0.0, 0.0, 0.0, 1.0)
);
uniform mat4 projection = mat4(
  vec4(1.0, 0.0, 0.0, 0.0),
  vec4(0.0, 1.0, 0.0, 0.0),
  vec4(0.0, 0.0, 1.0, 0.0),
  vec4(0.0, 0.0, 0.0, 1.0)
);

out VS_OUT {
  vec2 tex_uv;
} vs_out;

void main()
{
  vs_out.tex_uv = uv;

  vec3 camera_right_world = vec3(
    view[0][0],
    view[1][0],
    view[2][0]
  );
  vec3 camera_up_world = vec3(
    view[0][1],
    view[1][1],
    view[2][1]
  );

  vec3 vert_pos_world =
    center_pos_world +
    camera_right_world * position.x * scale +
    camera_up_world * position.y * scale;

  gl_Position = projection * vec4(vert_pos_world, 1.0);
}