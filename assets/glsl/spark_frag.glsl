#version 330 core

uniform vec3 start_color = vec3(0.15, 0.9, 0.05);
uniform vec3 end_color = vec3(0.1, 0.03, 0);

out vec4 FragColor;

void main()
{
  // FragColor = vec4(
  //   mix(
  //     start_color,
  //     end_color,
  //     length(velocity) / 10.0
  //   ),
  //   1.0
  // );

  FragColor = vec4(1, 1, 1, 1);
}