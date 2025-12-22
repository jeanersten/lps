#include "LPSPCH.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Object3D.hpp"

namespace LPS
{
  Light::Light(const Camera* camera)
    : Object3D(
        glm::vec3{ 1.0f, 1.0f, 1.0f },
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
    glm::mat4 model{ 1.0f };

    model = glm::translate(model, m_position);

    m_shader.Use();

    m_shader.SetUniformMat4f("u_View", m_camera->GetViewMatrix());
    m_shader.SetUniformMat4f("u_Projection", m_camera->GetProjectionMatrix());
    m_shader.SetUniformMat4f("u_Model", model);

    Object3D::Draw();

    m_shader.Unuse();
  }
}
