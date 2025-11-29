#include "LPSPCH.hpp"
#include "Object.hpp"

namespace LPS
{
  Object::Object(glm::vec2 position, glm::vec2 size, glm::vec4 color)
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
                           color}); 
      
    // bottom right (1)
    m_vertices.push_back({{position.x + size.x, position.y - size.y, 0.0f},
                           color}); 

    // top left (2)
    m_vertices.push_back({{position.x, position.y, 0.0f},
                           color}); 

    // bottom left (3)
    m_vertices.push_back({{position.x, position.y - size.y, 0.0f},
                           color}); 
        
    GLuint indices[]
    {
      0, 1, 2,
      2, 1, 3
    };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex),
                 m_vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
                 indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  void Object::Draw()
  {
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }
}
