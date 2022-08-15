#version 330 core
// TODO: get some better naming in here
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

out VS_OUT {
  vec3 mvp_frag_pos;
  vec3 mv_normal;
  vec2 tex_uv;
  mat4 projection;
} vs_out;

void main()
{
  vs_out.tex_uv = uv;
  vs_out.projection = perspective;
  vs_out.mvp_frag_pos = vec3(model * vec4(position, 1.0));
  vs_out.mv_normal = normalize(normals_model * local_normal);

  gl_Position = perspective * view * model * vec4(position, 1.0);
}