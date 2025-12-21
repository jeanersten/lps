#include "LPSPCH.hpp"
#include "Ball.hpp"
#include "Object3D.hpp"

namespace LPS
{
  Ball::Ball(const Camera* camera, const Light* light)
    : Object3D(
        glm::vec3{ 0.0f, 0.0f, 0.0f }, // Not currently used
        glm::vec3{ 0.5f, 0.5f, 0.5f },
        glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f },
        "assets/model/Sphere.obj",
        ""
      )
    , m_camera(camera)
    , m_light(light)
    , m_shader(
        "assets/shader/object_vertex.glsl",
        "assets/shader/object_fragment.glsl"
      )
  {}

  void Ball::Draw()
  {
    glm::mat4 model{ 1.0f };

    model = glm::translate(model, m_position);

    m_shader.Use();

    m_shader.SetUniformMat4f("u_View", m_camera->GetViewMatrix());
    m_shader.SetUniformMat4f("u_Projection", m_camera->GetProjectionMatrix());
    m_shader.SetUniformMat4f("u_Model", model);
    m_shader.SetUniformInt("u_Sampler", 0);
    m_shader.SetUniformVec3f("u_LightColor", m_light->GetColor());
    m_shader.SetUniformVec3f("u_LightPosition", m_light->GetPosition());
    m_shader.SetUniformVec3f("u_ViewPosition", m_camera->GetPosition());

    Object3D::Draw();

    m_shader.Unuse();
  }
}
