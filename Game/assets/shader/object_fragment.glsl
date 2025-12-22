#version 330 core

in vec3 Position;
in vec3 Normal;
in vec4 Color;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D u_Sampler;
uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;
uniform vec3 u_ViewPosition;

void main()
{
  float ambient_strength = 0.1f;
  vec3 ambient           = ambient_strength * u_LightColor;

  vec3 norm      = normalize(Normal);
  vec3 light_dir = normalize(u_LightPosition - Position);
  float diff     = max(dot(norm, light_dir), 0.0f);
  vec3 diffuse   = diff * u_LightColor;

  vec3 view_dir           = normalize(u_ViewPosition - Position);
  vec3 reflect_dir        = reflect(-light_dir, norm);
  float spec              = pow(max(dot(view_dir, reflect_dir), 0.0f), 32);
  float specular_strength = 0.5f;
  vec3 specular           = specular_strength * spec * u_LightColor;

  vec3 lighting = (ambient + diffuse + specular);

  vec3 albedo = vec3(texture(u_Sampler, TexCoord).rgb * Color.rgb);

  FragColor = vec4(albedo * lighting, Color.a);
}
