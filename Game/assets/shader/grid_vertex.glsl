#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texcoord;

out vec4 Color;
out vec2 TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform float u_Size;

void main()
{
  Color = color;
  TexCoord = texcoord * u_Size;

  gl_Position = u_Projection * u_View * u_Model * vec4(pos * u_Size, 1.0f);
}
