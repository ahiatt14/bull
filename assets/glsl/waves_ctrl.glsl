#version 410 core

layout (vertices = 4) out;

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

in gl_PerVertex {
  vec4 gl_Position;
  float gl_PointSize;
  float gl_ClipDistance[];
} gl_in[gl_MaxPatchVertices];

// TODO: lol
in vec2 tex_uv[];
out vec2 texture_uv[];

void main() {

  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
  texture_uv[gl_InvocationID] = tex_uv[gl_InvocationID];

  if (gl_InvocationID != 0) return;

  const int MIN_TESS = 1;
  const int MAX_TESS = 32;
  const float MIN_DIST = 50;
  const float MAX_DIST = 300;

  vec4 mv_pos0 = view * model * gl_in[0].gl_Position;
  vec4 mv_pos1 = view * model * gl_in[1].gl_Position;
  vec4 mv_pos2 = view * model * gl_in[2].gl_Position;
  vec4 mv_pos3 = view * model * gl_in[3].gl_Position;

  float dist0 = clamp((abs(mv_pos0.z)-MIN_DIST) / (MAX_DIST-MIN_DIST), 0., 1.);
  float dist1 = clamp((abs(mv_pos1.z)-MIN_DIST) / (MAX_DIST-MIN_DIST), 0., 1.);
  float dist2 = clamp((abs(mv_pos2.z)-MIN_DIST) / (MAX_DIST-MIN_DIST), 0., 1.);
  float dist3 = clamp((abs(mv_pos3.z)-MIN_DIST) / (MAX_DIST-MIN_DIST), 0., 1.);

  float tess_lvl0 = mix(MAX_TESS, MIN_TESS, min(dist2, dist0));
  float tess_lvl1 = mix(MAX_TESS, MIN_TESS, min(dist0, dist1));
  float tess_lvl2 = mix(MAX_TESS, MIN_TESS, min(dist1, dist3));
  float tess_lvl3 = mix(MAX_TESS, MIN_TESS, min(dist3, dist2));

  gl_TessLevelOuter[0] = tess_lvl0;
  gl_TessLevelOuter[1] = tess_lvl1;
  gl_TessLevelOuter[2] = tess_lvl2;
  gl_TessLevelOuter[3] = tess_lvl3;

  gl_TessLevelInner[0] = max(tess_lvl1, tess_lvl3);
  gl_TessLevelInner[1] = max(tess_lvl0, tess_lvl2);
}