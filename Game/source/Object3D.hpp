#pragma once

#include "Interface.hpp"
#include <GLAD/gl.h>
#include <GLM/glm.hpp>
#include <filesystem>
#include <vector>

namespace LPS
{
  class Object3D : public IDrawable
  {
  public:
    Object3D(glm::vec3 position, glm::vec3 size, glm::vec4 color,
             const std::filesystem::path& texture_path);
    virtual ~Object3D();

    void Draw() override;

  protected:
    struct Vertex
    {
      glm::vec3 pos;
      glm::vec4 color;
      glm::vec2 tex_coords;
    };

    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
    GLuint m_tex;

    std::vector<Vertex> m_vertices;
    glm::vec3 m_position;
    glm::vec3 m_size;
    glm::vec4 m_color;
  };
}
