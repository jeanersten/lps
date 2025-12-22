#version 330 core

in vec4 Color;
in vec2 TexCoord;

out vec4 FragColor;

uniform float u_Size;
uniform float u_CellSize;
uniform float u_SubcellSize;

void main()
{
  vec2 cell_coords = mod(TexCoord, u_CellSize) / u_CellSize;
  vec2 subcell_coords = mod(TexCoord, u_SubcellSize) / u_SubcellSize;

  FragColor = vec4(subcell_coords, 0.0f, 1.0f);
}
