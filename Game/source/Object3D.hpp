#pragma once

#include "Interface.hpp"
#include <GLAD/gl.h>
#include <glm/glm.hpp>
#include <filesystem>
#include <vector>

namespace LPS
{
  class Object3D : public IDrawable
  {
  public:
    Object3D(glm::vec3 position, glm::vec3 size, glm::vec4 color,
             const std::filesystem::path& texture_path);
    Object3D(glm::vec3 position, glm::vec3 size, glm::vec4 color,
             const std::filesystem::path& model_path,
             const std::filesystem::path& texture_path);
    virtual ~Object3D();

    void Draw() override;

    inline glm::vec3 GetPosition() const
    {
      return m_position;
    }

    inline glm::vec4 GetColor() const
    {
      return m_color;
    }

    inline void SetPosition(glm::vec3 val)
    {
      m_position = val;
    }

    inline void SetRotation(glm::vec3 val)
    {
      m_rotation = val;
    }

    inline void SetScale(glm::vec3 val)
    {
      m_scale = val;
    }

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
      glm::vec3 normal;
      glm::vec4 color;
      glm::vec2 tex_coord;
    };

    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
    glm::mat4 m_model;

    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    glm::vec3 m_size;
    glm::vec4 m_color;

    bool m_frame_mode;
  };
}
