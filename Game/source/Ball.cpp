#include "LPSPCH.hpp"
#include "Ball.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Object3D.hpp"

namespace LPS
{
  Ball::Ball(const Camera* camera, const std::vector<Light*>& lights)
    : Object3D(
        glm::vec3{ 0.0f, 0.0f, 0.0f },
        glm::vec3{ 1.0f, 1.0f, 1.0f },
        glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f },
        "assets/model/Sphere.obj",
        ""
      )
    , debug_panel(nullptr)
    , m_camera(camera)
    , m_lights(lights)
    , m_shader(
        "assets/shader/object_vertex.glsl",
        "assets/shader/object_fragment.glsl"
      )
  {}

  void Ball::Draw()
  {
    m_shader.Use();

    m_shader.SetUniformMat4f("u_View", m_camera->GetViewMatrix());
    m_shader.SetUniformMat4f("u_Projection", m_camera->GetProjectionMatrix());
    m_shader.SetUniformMat4f("u_Model", m_model);
    m_shader.SetUniformInt("u_Sampler", 0);
    m_shader.SetUniformInt("u_Lighting",
                           static_cast<int>(debug_panel->lighting));
    m_shader.SetUniformInt("u_LightCount", m_lights.size());
    for (int i = 0; i < m_lights.size(); i++)
    {
      m_shader.SetUniformVec3f("u_Lights[" + std::to_string(i) + "].position",
                               m_lights[i]->GetPosition());
      m_shader.SetUniformVec3f("u_Lights[" + std::to_string(i) + "].color",
                               m_lights[i]->GetColor());
    }
    m_shader.SetUniformFloat("u_LightLinear", debug_panel->light_linear);
    m_shader.SetUniformFloat("u_LightQuadratic", debug_panel->light_quadratic);
    m_shader.SetUniformVec3f("u_ViewPosition", m_camera->GetPosition());

    Object3D::Draw();

    m_shader.Unuse();
  }
}
