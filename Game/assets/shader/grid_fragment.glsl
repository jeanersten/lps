#version 330 core

in vec3 Position;
in vec4 Color;
in vec2 GridCoord;

out vec4 FragColor;

uniform float u_CellSize;
uniform float u_SubcellSize;
uniform float u_CellThickness;
uniform float u_SubcellThickness;
uniform vec4 u_CellColor;
uniform vec4 u_SubcellColor;
uniform vec3 u_ViewPosition;
uniform vec2 u_Size;
uniform float u_HeightFadeRatio;
uniform float u_MinFadePercent;
uniform float u_MaxFadePercent;

void main()
{
  vec2 ncell_coord    = mod(GridCoord, u_CellSize) / u_CellSize;
  vec2 nsubcell_coord = mod(GridCoord, u_SubcellSize) / u_SubcellSize;

  vec2 dv = fwidth(GridCoord);
  vec2 nhcell_thickness    = (u_CellThickness + dv) / u_CellSize * 0.5f;
  vec2 nhsubcell_thickness = (u_SubcellThickness + dv) / u_SubcellSize * 0.5f;

  vec4 final_color = vec4(0.0f);

  if (any(lessThan(nsubcell_coord, nhsubcell_thickness)) ||
      any(greaterThan(nsubcell_coord, 1.0f - nhsubcell_thickness)))
  {
    final_color = u_SubcellColor;
  }
  if (any(lessThan(ncell_coord, nhcell_thickness)) ||
      any(greaterThan(ncell_coord, 1.0f - nhcell_thickness)))
  {
    final_color = u_CellColor;
  }
  
  float min_fade_distance = min(u_Size.x, u_Size.y) * u_MinFadePercent;
  float max_fade_distance = max(u_Size.x, u_Size.y) * u_MaxFadePercent;
  float dto_camera    = length(GridCoord - u_ViewPosition.xz);
  float fade_distance = abs(u_ViewPosition.y) * u_HeightFadeRatio;

  fade_distance = max(fade_distance, min_fade_distance);
  fade_distance = min(fade_distance, max_fade_distance);
 
  float opacity_falloff =  smoothstep(1.0, 0.0, dto_camera / fade_distance);

  FragColor = final_color * Color * opacity_falloff;
}
