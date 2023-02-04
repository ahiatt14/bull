#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform float seconds_since_activation;

uniform float speed = 2;
uniform float wavelength = 0.5;
uniform float amplitude = 0.3;

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
    sin(wavelength * gl_Position.y + seconds_since_activation * speed)
    * amplitude;
  gs_out.tex_uv = gs_in[0].tex_uv;
  EmitVertex();

  gl_Position = gl_in[1].gl_Position;
  gl_Position.x =
    gl_Position.x +
    sin(wavelength * gl_Position.y + seconds_since_activation * speed)
    * amplitude;
  gs_out.tex_uv = gs_in[1].tex_uv;
  EmitVertex();
  
  gl_Position = gl_in[2].gl_Position;
  gl_Position.x =
    gl_Position.x +
    sin(wavelength * gl_Position.y + seconds_since_activation * speed)
    * amplitude;
  gs_out.tex_uv = gs_in[2].tex_uv;
  EmitVertex();

  EndPrimitive();
}