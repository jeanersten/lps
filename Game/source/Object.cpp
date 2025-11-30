#include "LPSPCH.hpp"
#include "Log.hpp"
#include "Object.hpp"

namespace LPS
{
  Object::Object(glm::vec2 position, glm::vec2 size, glm::vec4 color,
                 const std::filesystem::path& path)
    : m_vao(0)
    , m_vbo(0)
    , m_vertices()
    , m_position(position)
    , m_size(size)
    , m_color(color)
  {
    m_vertices.reserve(4);

    // top right (0)
    m_vertices.push_back({{position.x + size.x, position.y, 0.0f},
                           color, {1.0f, 1.0f}}); 
      
    // bottom right (1)
    m_vertices.push_back({{position.x + size.x, position.y - size.y, 0.0f},
                           color, {1.0f, 0.0f}}); 

    // top left (2)
    m_vertices.push_back({{position.x, position.y, 0.0f},
                           color, {0.0f, 1.0f}}); 

    // bottom left (3)
    m_vertices.push_back({{position.x, position.y - size.y, 0.0f},
                           color, {0.0f, 0.0f}}); 
        
    GLuint indices[]
    {
      0, 1, 2,
      2, 1, 3
    };

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
                 indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)offsetof(Vertex, tex_coords));
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

    LPS_ASSERT(data != nullptr, "Failed to load texture from file.");

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  Object::~Object()
  {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteTextures(1, &m_tex);
  }

  void Object::Draw()
  {
    glBindTexture(GL_TEXTURE_2D, m_tex);
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }
}
