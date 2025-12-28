#include "LPSPCH.hpp"
#include "Log.hpp"
#include "Object2D.hpp"

namespace LPS
{
  Object2D::Object2D(glm::vec3 position, glm::vec2 size, glm::vec4 color,
                     const std::filesystem::path& path)
    : m_vao(0)
    , m_vbo(0)
    , m_ebo(0)
    , m_tex(0)
    , m_position(position)
    , m_rotation(0.0f, 0.0f, 0.0f)
    , m_scale(1.0f, 1.0f)
    , m_model(1.0f)
    , m_vertices()
    , m_indices()
    , m_size(size)
    , m_color(color)
    , m_frame_mode(false)
  {
    m_vertices.reserve(4);

    float x1{ -size.x };
    float y1{  size.y };
    float x2{  size.x };
    float y2{ -size.y };

    m_vertices.push_back({ { x2, y1, 0.0f }, color, { 1.0f, 1.0f } });
    m_vertices.push_back({ { x2, y2, 0.0f }, color, { 1.0f, 0.0f } });
    m_vertices.push_back({ { x1, y1, 0.0f }, color, { 0.0f, 1.0f } });
    m_vertices.push_back({ { x1, y2, 0.0f }, color, { 0.0f, 0.0f } });

    GLuint indices[]
    {
      0, 1, 2,
      2, 1, 3
    };

    size_t indices_size{ sizeof(indices) / sizeof(GLuint) };

    m_vertices.reserve(indices_size);

    for (int i = 0; i < indices_size; i++)
    {
      m_indices.push_back(indices[i]);
    }

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glGenTextures(1, &m_tex);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex),
                 m_vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
                 m_indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)offsetof(Vertex, tex_coord));
    glEnableVertexAttribArray(2);

    glBindTexture(GL_TEXTURE_2D, m_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                    GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);

    int tex_width{ 0 };
    int tex_height{ 0 };

    stbi_set_flip_vertically_on_load(1);

    GLubyte* data{ stbi_load(std::filesystem::absolute(path).string().c_str(),
                             &tex_width, &tex_height, nullptr, 3) };

    if (data != nullptr)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0,
                   GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      stbi_image_free(data);
    }
    else
    {
      GLubyte red = static_cast<GLubyte>(
        std::clamp(m_color.r, 0.0f, 1.0f) * 255.0f
      );

      GLubyte green = static_cast<GLubyte>(
        std::clamp(m_color.g, 0.0f, 1.0f) * 255.0f
      );

      GLubyte blue = static_cast<GLubyte>(
        std::clamp(m_color.b, 0.0f, 1.0f) * 255.0f
      );

      GLubyte fallback_color[3] { red, green, blue };

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0,
                   GL_RGB, GL_UNSIGNED_BYTE, fallback_color);
      glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  Object2D::~Object2D()
  {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteTextures(1, &m_tex);
  }

  void Object2D::Draw()
  {
    m_model = 1.0f;
    m_model = glm::translate(m_model, m_position);
    m_model = glm::rotate(m_model, glm::radians(m_rotation.x),
                          glm::vec3{ 1.0f, 0.0f, 0.0f });
    m_model = glm::rotate(m_model, glm::radians(m_rotation.y),
                          glm::vec3{ 0.0f, 1.0f, 0.0f });
    m_model = glm::rotate(m_model, glm::radians(m_rotation.z),
                          glm::vec3{ 0.0f, 0.0f, 1.0f });
    m_model = glm::scale(m_model, glm::vec3{ m_scale.x, m_scale.y, 1.0f });

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_tex);
    glBindVertexArray(m_vao);
    glPolygonMode(GL_FRONT_AND_BACK, m_frame_mode ? GL_LINE : GL_FILL);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
}
