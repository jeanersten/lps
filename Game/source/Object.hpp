#pragma once

#include "Interface.hpp"
#include <GLAD/gl.h>
#include <GLM/glm.hpp>
#include <vector>

namespace LPS
{
  class Object : public IDrawable
  {
  public:
    Object(glm::vec2 position, glm::vec2 size, glm::vec4 color);
    virtual ~Object() = default;

    void Draw() override;

  protected:
    struct Vertex
    {
      glm::vec3 pos;
      glm::vec4 color;
    };

    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;

    std::vector<Vertex> m_vertices;
    glm::vec2 m_position;
    glm::vec2 m_size;
    glm::vec4 m_color;
  };
}
