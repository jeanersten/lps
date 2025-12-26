#include "LPSPCH.hpp"
#include "Camera.hpp"
#include "Grid.hpp"
#include "Object2D.hpp"

namespace LPS
{
  Grid::Grid(const Camera* camera)
    : Object2D(
        glm::vec3{ 0.0f, 0.0f, 0.0f },
        glm::vec2{ 150.0f, 150.0f },
        glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f },
        ""
      )
    , m_camera(camera)
    , m_shader(
        "assets/shader/grid_vertex.glsl",
        "assets/shader/grid_fragment.glsl"
      )
  {}

  void Grid::Draw()
  {
    glm::mat4 model{ 1.0f };

    model = glm::translate(model, m_position);
    model = glm::rotate(model, glm::radians(-90.0f),
                        glm::vec3{ 1.0f, 0.0f, 0.0f });

    m_shader.Use();

    m_shader.SetUniformMat4f("u_View", m_camera->GetViewMatrix());
    m_shader.SetUniformMat4f("u_Projection", m_camera->GetProjectionMatrix());
    m_shader.SetUniformMat4f("u_Model", model);
    m_shader.SetUniformVec3f("u_ViewPosition", m_camera->GetPosition());
    m_shader.SetUniformFloat("u_CellSize", 1.0f);
    m_shader.SetUniformFloat("u_SubcellSize", 0.2f);
    m_shader.SetUniformFloat("u_CellThickness", 0.01f);
    m_shader.SetUniformFloat("u_SubcellThickness", 0.001f);
    m_shader.SetUniformVec4f("u_CellColor",
                             glm::vec4{ 0.75f, 0.75f, 0.75f, 1.0f });
    m_shader.SetUniformVec4f("u_SubcellColor",
                             glm::vec4{ 0.5f, 0.5f, 0.5f, 1.0f });
    m_shader.SetUniformVec2f("u_Size", m_size);
    m_shader.SetUniformFloat("u_HeightFadeRatio", 25.0f);
    m_shader.SetUniformFloat("u_MinFadePercent", 0.3f);
    m_shader.SetUniformFloat("u_MaxFadePercent", 0.8f);

    Object2D::Draw();

    m_shader.Unuse();
  }
}
