#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

in VS_OUT {
  vec4 endpoint;
} gs_in[];

void main() {

  vec4 pos = gl_in[0].gl_Position;

  vec4 _0 = pos;
  vec4 _1 = pos + gs_in[0].endpoint;
  vec4 _2 = _1 + vec4(cross(_1.xyz, _0.xyz), 0.0) * 0.002;

  gl_Position = _0;
  EmitVertex();

  gl_Position = _2;
  EmitVertex();

  gl_Position = _1;
  EmitVertex();

  EndPrimitive();
}