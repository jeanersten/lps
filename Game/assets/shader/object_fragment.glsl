#version 330 core

#define MAX_LIGHT 8

struct Light
{
  vec3 position;
  vec3 color;
};

in vec3 Position;
in vec3 Normal;
in vec4 Color;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D u_Sampler;
uniform bool u_Lighting;
uniform int u_LightCount;
uniform Light u_Lights[MAX_LIGHT];
uniform float u_LightLinear;
uniform float u_LightQuadratic;
uniform vec3 u_ViewPosition;

void main()
{
  vec3 albedo = texture(u_Sampler, TexCoord).rgb * Color.rgb;

  if (u_Lighting)
  {
    vec3 norm = normalize(Normal);
    vec3 view_dir = normalize(u_ViewPosition - Position);

    float ambient_strength = 0.15f;
    vec3 lighting = ambient_strength * vec3(1.0f);

    for (int i = 0; i < u_LightCount; i++)
    {
      vec3 light_to_frag = u_Lights[i].position - Position;
      float light_dist   = length(light_to_frag);
      vec3 light_dir     = normalize(light_to_frag);

      float attenuation = 1.0f / (1.0f + u_LightLinear * light_dist +
                                  u_LightQuadratic * light_dist * light_dist);

      float diffuse_factor = max(dot(norm, light_dir), 0.0f);
      vec3 diffuse         = diffuse_factor * u_Lights[i].color;

      vec3 halfway_dir = normalize(light_dir + view_dir);

      float specular_strength = 0.5f;

      float specular_factor = pow(max(dot(norm, halfway_dir), 0.0), 64.0);
      vec3 specular         = specular_strength * specular_factor *
                              u_Lights[i].color;

      lighting += (diffuse + specular) * attenuation;
    }

    FragColor = vec4(albedo * lighting, Color.a);
  }
  else
  {
    FragColor = vec4(albedo, Color.a);
  }
}
