#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 texcoords;

out vec3 Position;
out vec3 Normal;
out vec4 Color;
out vec2 TexCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
  Position = vec3(u_Model * vec4(pos, 1.0f));
  Normal = mat3(transpose(inverse(u_Model))) * normal;
  Color = color;
  TexCoords = texcoords;

  gl_Position = u_Projection * u_View * u_Model * vec4(pos, 1.0f);
}
