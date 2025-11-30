#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texcoords;

out vec4 VertexColor;
out vec2 VertexTexCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
  gl_Position = u_Projection * u_View * u_Model * vec4(pos, 1.0f);
  VertexColor = color;
  VertexTexCoords = texcoords;
}
