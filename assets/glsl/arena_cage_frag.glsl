#version 330 core

in vec2 TexCoord;

uniform sampler2D noise1;
uniform sampler2D noise2;

out vec4 FragColor;
void main()
{
  FragColor = vec4(texture(noise1, TexCoord));
}