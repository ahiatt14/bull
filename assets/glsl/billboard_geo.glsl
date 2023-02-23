#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

uniform float scale = 1.0;
uniform mat4 projection = mat4(
  vec4(1.0, 0.0, 0.0, 0.0),
  vec4(0.0, 1.0, 0.0, 0.0),
  vec4(0.0, 0.0, 1.0, 0.0),
  vec4(0.0, 0.0, 0.0, 1.0)
);
uniform mat4 rotation = mat4(
  vec4(1.0, 0.0, 0.0, 0.0),
  vec4(0.0, 1.0, 0.0, 0.0),
  vec4(0.0, 0.0, 1.0, 0.0),
  vec4(0.0, 0.0, 0.0, 1.0)
);

out VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} gs_out;

void main() {

  gl_Position =
    projection *
    (gl_in[0].gl_Position + rotation * vec4(-1, -1, 0, 0) * scale);
  gs_out.tex_uv = vec2(0, 0);
  EmitVertex();

  gl_Position =
    projection *
    (gl_in[0].gl_Position + rotation * vec4(1, -1, 0, 0) * scale);
  gs_out.tex_uv = vec2(1, 0);
  EmitVertex();

  gl_Position =
    projection *
    (gl_in[0].gl_Position + rotation * vec4(1, 1, 0, 0) * scale);
  gs_out.tex_uv = vec2(1, 1);
  EmitVertex();

  EndPrimitive();

  gl_Position =
    projection *
    (gl_in[0].gl_Position + rotation * vec4(-1, -1, 0, 0) * scale);
  gs_out.tex_uv = vec2(0, 0);
  EmitVertex();

  gl_Position =
    projection *
    (gl_in[0].gl_Position + rotation * vec4(1, 1, 0, 0) * scale);
  gs_out.tex_uv = vec2(1, 1);
  EmitVertex();

  gl_Position =
    projection *
    (gl_in[0].gl_Position + rotation * vec4(-1, 1, 0, 0) * scale);
  gs_out.tex_uv = vec2(0, 1);
  EmitVertex();

  EndPrimitive();
}