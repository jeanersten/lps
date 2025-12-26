#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 color;

out vec3 Position;
out vec4 Color;
out vec2 GridCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec3 u_ViewPosition;

void main()
{  
  Position = vec3(u_Model * vec4(pos, 1.0f));
  Position.xz += u_ViewPosition.xz;
  Color = color;
  GridCoord = Position.xz;
  
  gl_Position = u_Projection * u_View * vec4(Position, 1.0f);
}
