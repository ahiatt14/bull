#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform float total_elapsed_seconds;

uniform float speed = -0.5;
uniform float wavelength = 3.5;
uniform float amplitude = 1;

in VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} gs_in[];

out VS_OUT {
  vec3 world_frag_pos;
  vec3 normal;
  vec2 tex_uv;
} gs_out;

void main() {

  gl_Position = gl_in[0].gl_Position;
  gl_Position.x =
    gl_Position.x +
    sin(wavelength * gl_Position.y + total_elapsed_seconds * speed)
    * amplitude;
  gs_out.tex_uv = gs_in[0].tex_uv;
  EmitVertex();

  gl_Position = gl_in[1].gl_Position;
  gl_Position.x =
    gl_Position.x +
    sin(wavelength * gl_Position.y + total_elapsed_seconds * speed)
    * amplitude;
  gs_out.tex_uv = gs_in[1].tex_uv;
  EmitVertex();
  
  gl_Position = gl_in[2].gl_Position;
  gl_Position.x =
    gl_Position.x +
    sin(wavelength * gl_Position.y + total_elapsed_seconds * speed)
    * amplitude;
  gs_out.tex_uv = gs_in[2].tex_uv;
  EmitVertex();

  EndPrimitive();
}