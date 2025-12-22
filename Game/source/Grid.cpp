#include "LPSPCH.hpp"
#include "Camera.hpp"
#include "Grid.hpp"
#include "Object2D.hpp"

namespace LPS
{
  Grid::Grid(const Camera* camera)
    : Object2D(
        glm::vec3{ 0.0f, 0.0f, 0.0f },
        glm::vec2{ 1.0f, 1.0f },
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
    model = glm::rotate(model, glm::radians(90.0f),
                        glm::vec3{ 1.0f, 0.0f, 0.0f });

    m_shader.Use();

    m_shader.SetUniformMat4f("u_View", m_camera->GetViewMatrix());
    m_shader.SetUniformMat4f("u_Projection", m_camera->GetProjectionMatrix());
    m_shader.SetUniformMat4f("u_Model", model);
    m_shader.SetUniformFloat("u_Size", 100.0f);
    m_shader.SetUniformFloat("u_CellSize", 10.0f);
    m_shader.SetUniformFloat("u_SubcellSize", 1.0f);

    Object2D::Draw();

    m_shader.Unuse();
  }
}
