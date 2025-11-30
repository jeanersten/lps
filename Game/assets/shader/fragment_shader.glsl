#version 330 core

in vec4 VertexColor;
in vec2 VertexTexCoords;

out vec4 FragColor;

uniform sampler2D u_Sampler;

void main()
{
  FragColor = texture(u_Sampler, VertexTexCoords) * VertexColor;
}
