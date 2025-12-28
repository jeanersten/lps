#include "LPSPCH.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Object3D.hpp"

namespace LPS
{
  Light::Light(const Camera* camera)
    : Object3D(
        glm::vec3{ 0.0f, 0.0f, 0.0f },
        glm::vec3{ 0.25f, 0.25f, 0.25f },
        glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f },
        ""
      )
    , m_camera(camera)
    , m_shader(
        "assets/shader/light_vertex.glsl",
        "assets/shader/light_fragment.glsl"
      )
  {}

  void Light::Draw()
  {
    m_shader.Use();

    m_shader.SetUniformMat4f("u_View", m_camera->GetViewMatrix());
    m_shader.SetUniformMat4f("u_Projection", m_camera->GetProjectionMatrix());
    m_shader.SetUniformMat4f("u_Model", m_model);

    Object3D::Draw();

    m_shader.Unuse();
  }
}
