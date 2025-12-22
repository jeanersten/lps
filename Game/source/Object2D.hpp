#pragma once

#include "Interface.hpp"
#include <GLAD/gl.h>
#include <glm/glm.hpp>
#include <filesystem>
#include <vector>

namespace LPS
{
  class Object2D : public IDrawable
  {
  public:
    Object2D(glm::vec3 position, glm::vec2 size, glm::vec4 color,
             const std::filesystem::path& texture_path);
    virtual ~Object2D();

    void Draw() override;

    inline void SetDrawFrameMode(bool val)
    {
      m_frame_mode = val;
    }

  protected:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
    GLuint m_tex;

    struct Vertex
    {
      glm::vec3 pos;
      glm::vec4 color;
      glm::vec2 tex_coord;
    };

    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    glm::vec3 m_position;
    glm::vec2 m_size;
    glm::vec4 m_color;

    bool m_frame_mode;
  };
}
